//
// Created by sriramsrinivas@unomaha.edu on 1/9/20.
//


#include "SP_structure.hpp"
#include "readin_data.hpp"
#include "verify.hpp"
#include "delete_ops1.hpp"
#include "insert_ops1.hpp"
//#include "printout_others.hpp"
#include "pr.h"
#include <string>
int main(int argc, char *argv[])
{
    clock_t q, q1, q2,t;
    clock_t updateStartTime;
    clock_t updateEndTime;
    if ( argc <7) { cout << "INPUT ERROR:: 7 inputs required. First: filename \n Second: file with PR\n  Third: Set of changed edges \n Fourth: Number of nodes \n Fifth: Number of threads \n Sixth: Number of iterations\n Seventh: Output File name ";}
    //Check to see if file opening succeeded
    ifstream the_file1 ( argv[1] ); if (!the_file1.is_open() ) { cout<<"INPUT ERROR:: Could not open file\n";}
    ifstream the_file2 ( argv[2] ); if (!the_file2.is_open() ) { cout<<"INPUT ERROR:: Could not open file\n";}
    ifstream the_file3 ( argv[3] ); if (!the_file3.is_open() ) { cout<<"INPUT ERROR:: Could not open file\n";}
    int p = atoi(argv[5]);  //total number of threads per core
    int nodes=atoi(argv[4]);//total number of nodes


    /*
     * First Step is to read the PR value form the given input dataset
     *
     */

    PR_Comp CI;
    vector<PR_Comp> pageRank_Info;
    vector<PageRank_MetaInformation>pageRankCompleteInformation;
    pageRank_Info.resize(nodes,CI);


SCC_Network initialNetwork;

    q=clock();
    VertexProp VP;
    initialNetwork.resize(nodes, VP);
    readin_PageRank(argv[2],&initialNetwork,&pageRank_Info);
    q=clock()-q;

    cout << "Total Time for Reading PageRank "<< ((float)q)/CLOCKS_PER_SEC <<"\n";
    /**=============================================**/

    /**=============================================**/
    /*** Reading in Network at Time Step 0 ***/
    /***============================================**/
    //Obtain the list of edges.
    q=clock();
    readin_network(&initialNetwork,argv[1]);
    //Update batch size in parallel
    batch_update(&initialNetwork,p);
    q=clock()-q;

    cout << "Total Time for Reading Network"<< ((float)q)/CLOCKS_PER_SEC <<"\n";
    /**=============================================**/

// Update all the meta information

   updatePageRankMetaInformation(&initialNetwork,&pageRank_Info,&pageRankCompleteInformation);
   compute_d_ValueforGivenPageRank(&pageRankCompleteInformation);
   // call printPageRankCompleteInformationInitial only for debug
   //printPageRankCompleteInformationInitial(&initialNetwork,&pageRank_Info,&pageRankCompleteInformation);

   // Identify the nodes which needs to be updated
   // process CHanged Edges
    int b=1;
    readin_changes(argv[3], b,&initialNetwork,&pageRank_Info,&pageRankCompleteInformation);
    batch_update(&initialNetwork,p);
   // printPageRankCompleteInformationInitial(&initialNetwork,&pageRank_Info,&pageRankCompleteInformation);

    int maxIterations=atoi(argv[6]);
    updateStartTime=clock();
    updatePageRank(&initialNetwork,&pageRank_Info,&pageRankCompleteInformation,&p,&maxIterations);
    updateEndTime=clock()-updateStartTime;
    float updateTime=float(((float)updateEndTime)/CLOCKS_PER_SEC);
    cout <<fixed<< "Total Time for updating Network "<< updateTime <<"\n";
    printFinalPageRankValuesForAllNodes(&pageRankCompleteInformation);
    std::string outputFileName= argv[7];
    cout <<outputFileName<<"\n";
    printFinalPageRankValuesForAllNodesinAFile(&pageRankCompleteInformation,&outputFileName, &updateTime);


   // Mark the nodes and its neighbor for update
   // keep updating the neighbor in asynchrony manner


}
