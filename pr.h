//
// Created by sriramsrinivas@unomaha.edu on 1/9/20.
//

#ifndef PR_PR_H
#define PR_PR_H
#include <limits>
#include <cmath>
typedef std::numeric_limits< double > dbl;
#include "SP_structure.hpp"
#include <omp.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <queue>

typedef  vector<PageRank_MetaInformation> PageRank_Network;
void updatePageRankMetaInformation(SCC_Network *X,vector<PR_Comp> * pageRank_Info,vector<PageRank_MetaInformation> *pageRankCompleteInformation)
{
    //cout<<"I am here for updating Page Rank meta Information"<<"\n";


    for (int i=0;i<X->size();i++) {
        pageRankCompleteInformation->at(i).pageRank=pageRank_Info->at(i).pageRank;
        pageRankCompleteInformation->at(i).id=i;
        for (int j = 0; j < X->at(i).In_Neigh.size(); j++) {
            int_double dummy;
            //cout << X->at(i).In_Neigh[j].first << "\t";
            dummy.first = X->at(i).In_Neigh[j].first;
            dummy.second = pageRank_Info->at(j).pageRank;
            pageRankCompleteInformation->at(i).outsideConnnection.push_back(dummy);

        }
             pageRankCompleteInformation->at(i).outsideConnectionSize=X->at(i).In_Neigh.size();


        for(int j=0;j<X->at(i).Out_Neigh.size();j++) {

               int_double dummy;
               dummy.first=X->at(i).Out_Neigh[j].first;
               dummy.second=pageRank_Info->at(j).pageRank;
            pageRankCompleteInformation->at(i).inConnnection.push_back(dummy);


        }
             pageRankCompleteInformation->at(i).inConnectionSize=X->at(i).Out_Neigh.size();

        pageRankCompleteInformation->at(i).afterProcessigCEInConnectionSize=pageRankCompleteInformation->at(i).inConnectionSize;
        pageRankCompleteInformation->at(i).afterProcessingCEOutsideConnectionSize=pageRankCompleteInformation->at(i).outsideConnectionSize;

        //cout << "\n";
    }
}



void computeValueOfNode(vector<PageRank_MetaInformation> *pageRankCompleteInformation,int *i)
{
    double value=0.00;
    //cout <<i<<"\t";
    //cout <<pageRankCompleteInformation->at(i).inConnnection.size()<<"\t";

    //
    for(int j=0;j< pageRankCompleteInformation->at(*i).inConnnection.size();j++)
    {
      if(pageRankCompleteInformation->at(*i).inConnnection.at(j).first!=-1) {
          value += (pageRankCompleteInformation->at(pageRankCompleteInformation->at(*i).inConnnection.at(j).first).pageRank) /
                   (pageRankCompleteInformation->at(pageRankCompleteInformation->at(*i).inConnnection.at(j).first).afterProcessingCEOutsideConnectionSize);
      }
    }
    pageRankCompleteInformation->at(*i).intermediateValue=value;
    //cout<<value<<"\n";

}

/*
 *
 * d= (Pr-V)/(1/n-v)
 * where V= {PR[v]/p(V)
 */


void compute_d_ValueforGivenPageRank(vector<PageRank_MetaInformation> *pageRankCompleteInformation)
{
     double d_value=0.00;

    for(int i=0;i<pageRankCompleteInformation->size();i++) {

        computeValueOfNode(pageRankCompleteInformation, &i);
       double tmpvalue1=pageRankCompleteInformation->at(i).pageRank-pageRankCompleteInformation->at(i).intermediateValue;
       double tmpvalue2=(1.00/(pageRankCompleteInformation->size()))-pageRankCompleteInformation->at(i).intermediateValue;
       d_value = abs(tmpvalue1) / abs(tmpvalue2);
       pageRankCompleteInformation->at(i).dValue=d_value;


    }
    }


//void updateValueOfNode(vector<PageRank_MetaInformation> *pageRankCompleteInformation, int *nodetoBeupdated)
//{
//
//}

void  printPageRankCompleteInformationInitial(SCC_Network *X,vector<PR_Comp> * pageRank_Info,vector<PageRank_MetaInformation> *pageRankCompleteInformation)
{
    for (int i=0;i< X->size();i++) {
        cout << i << "\t";

        cout<<pageRankCompleteInformation->at(i).pageRank<<"\t";
        cout<<"Internal: \t";
        for(int j=0;j< pageRankCompleteInformation->at(i).inConnnection.size();j++)
        {
            cout <<pageRankCompleteInformation->at(i).inConnnection.at(j).first<<"\t";
        }
        cout<<"External:  \t";

        for(int j=0;j< pageRankCompleteInformation->at(i).outsideConnnection.size();j++)
        {
            cout <<pageRankCompleteInformation->at(i).outsideConnnection.at(j).first<<"\t";
        }

        cout <<"size: \t";

        cout <<pageRankCompleteInformation->at(i).outsideConnectionSize<<"\t";
        cout <<pageRankCompleteInformation->at(i).inConnectionSize<<"\t";

        cout <<"Level: \t";
        cout <<pageRankCompleteInformation->at(i).level<<"\t";
//        cout <<pageRankCompleteInformation->at(i).inConnectionSize<<"\t";

        cout <<"\n";
    }


}


void updatePageRankUpdateFlagForNeighbors (vector<PageRank_MetaInformation> *pageRankCompleteInformation, int *nodetoBeupdated, int *totalNumberofNodesMarkedforUpdate,vector<int> *nodesMarkedforUpdate)
{
   int count=*totalNumberofNodesMarkedforUpdate;

    for(int j=0;j< pageRankCompleteInformation->at(*nodetoBeupdated).outsideConnnection.size();j++) {
        int nodetoMark = pageRankCompleteInformation->at(*nodetoBeupdated).outsideConnnection.at(j).first;

        if (nodetoMark != -1) {
            if (!pageRankCompleteInformation->at(nodetoMark).updateFlag) {
              //check for neighbors PR significance
//              for(int m=0;m<pageRankCompleteInformation->at(nodetoMark).inConnnection.size();m++)
//              {
//
//                  if (pageRankCompleteInformation->at(nodetoMark).inConnnection.at(m).first!=-1)
//                  {
//                      if(pageRankCompleteInformation->at(nodetoMark).inConnnection.at(m).second)
//                  }
//
//
//              }

                pageRankCompleteInformation->at(nodetoMark).updateFlag = true;
                nodesMarkedforUpdate->push_back(nodetoMark);
                count++;
               // *totalNumberofNodesMarkedforUpdate=count;
               // cout<<"updateafterrea"<<nodetoMark<<"\n";
            }
        }
    }
    *totalNumberofNodesMarkedforUpdate=count;

}

void  computePageRankofNodetobeUpdated(vector<PageRank_MetaInformation> *pageRankCompleteInformation, int *nodetoBeupdated)
{
    /*
     *
     * Don't compute the values, just update the existing values
     * still need to be optimized
     */


        computeValueOfNode(pageRankCompleteInformation, nodetoBeupdated);

   // updateValueOfNode(pageRankCompleteInformation,nodetoBeupdated)
    pageRankCompleteInformation->at(*nodetoBeupdated).pageRank=((pageRankCompleteInformation->at(*nodetoBeupdated).dValue)/(pageRankCompleteInformation->size())

            +(1-pageRankCompleteInformation->at(*nodetoBeupdated).dValue)*(pageRankCompleteInformation->at(*nodetoBeupdated).intermediateValue));
    double valueforCheckingSignificance= abs(pageRankCompleteInformation->at(*nodetoBeupdated).previousIterationPageRankValue-(pageRankCompleteInformation->at(*nodetoBeupdated).pageRank));
    // check for 10^-3 aand 10^-7
    if(valueforCheckingSignificance>0.00000001)
    {
        pageRankCompleteInformation->at(*nodetoBeupdated).updateFlag=false;
    }else
        {
            pageRankCompleteInformation->at(*nodetoBeupdated).previousIterationPageRankValue= pageRankCompleteInformation->at(*nodetoBeupdated).pageRank;

//            for (int m=0;m<pageRankCompleteInformation->at(*nodetoBeupdated).inConnnection.size();m++)
    }

}


/**keep counter for number of nodes getting updted, iterations and email Galois, more number of changes  **/

/*
     * PR(v)= (d(given in preprocessing file )/n(number of nodes in the network) + (1-d)*[PR(v1)/O(V1)+PR(v2)/)(V2)])

     *   PR(v1)/O(v1)+PR(v2)/ O(v2) == Value which is stored in the meta information structure
     *    V1 and v2 have edges which reach V
     *
     *    not now
     *
     */

void updatePageRank(SCC_Network *X,vector<PR_Comp> * pageRank_Info,vector<PageRank_MetaInformation> *pageRankCompleteInformation, int *p, int *maxIterations, int *totalNumberofNodesMarkedforUpdate, vector<int> *nodesMarkedforUpdate)
{
  cout<<*maxIterations<<"\n";
  vector<double> previousIterationValue;
  int counter=0;
// int count=*totalNumberofNodesMarkedforUpdate;
  bool change=true;
    while(counter<*maxIterations && change==true) {
change=false;
 #pragma omp parallel for schedule(dynamic) num_threads(*p)
        for (int iterator = 0; iterator < pageRankCompleteInformation->size(); iterator++) {

            /*
             * Add && pageRankCompleteInformation->at(i).vertexLock==false
             * need to discuss with SB, as this can again lead to atomic operation
             */

              // add condition for level
            if (pageRankCompleteInformation->at(iterator).updateFlag== true && pageRankCompleteInformation->at(iterator).level==counter)
            { change=true;

                //pageRankCompleteInformation->at(i).vertexLock=true;
                int nodetoBeupdated=pageRankCompleteInformation->at(iterator).id;
                computePageRankofNodetobeUpdated(pageRankCompleteInformation,&nodetoBeupdated);
                // check for  value true  again and see if there is any significant change
                if(pageRankCompleteInformation->at(iterator).updateFlag==true) {
                    updatePageRankUpdateFlagForNeighbors(pageRankCompleteInformation, &nodetoBeupdated,
                                                         totalNumberofNodesMarkedforUpdate, nodesMarkedforUpdate);
                }
            }


        }

        counter++;

    }
    cout<<"counterStatus"<<counter<<"\n";
}

void  printFinalPageRankValuesForAllNodes(vector<PageRank_MetaInformation> *pageRankCompleteInformation)
{
    for(int i=0;i<pageRankCompleteInformation->size();i++)
    {
        cout <<pageRankCompleteInformation->at(i).id<<"\t";
        cout<<pageRankCompleteInformation->at(i).pageRank<<"\n";
    }
}


void printFinalPageRankValuesForAllNodesinAFile(vector<PageRank_MetaInformation> *pageRankCompleteInformation, std::string *fileName, float *updateTime, int * totalNumberofNodesMarkedforUpdate )
{
    ofstream myfile;
    myfile.open (*fileName);
    myfile<<"Total time to Update the Network:"<<" " <<*updateTime<<"\n";
    myfile << "Here is the Output!.\n";
    myfile<<"Total number of nodes marked for update:"<<*totalNumberofNodesMarkedforUpdate<<"\n";

    for(int i=0;i<pageRankCompleteInformation->size();i++)
    {
        myfile<<pageRankCompleteInformation->at(i).id<<"\t";
        myfile<<std::setprecision(15)<<pageRankCompleteInformation->at(i).pageRank<<"\n";
    }

    myfile.close();
}



void  custom_build_traversal(queue<int> *myqueue,vector<PageRank_MetaInformation> *pageRankCompleteInformation)
{
    while(!myqueue->empty())
    {

//        cout<<"myqueu" <<myqueue->front()<<"\n";
        for(int j=0;j<pageRankCompleteInformation->at(myqueue->front()).outsideConnnection.size();j++)
        {
//            cout <<"j--"<<pageRankCompleteInformation->at(myqueue->front()).afterProcessingCEOutsideConnectionSize<<"\n";

            int value=pageRankCompleteInformation->at(myqueue->front()).outsideConnnection.at(j).first;
//            cout <<"vv="<<value<<"\n";
            if(value!=-1) {
                if (pageRankCompleteInformation->at(value).visited == false) {
                    pageRankCompleteInformation->at(value).visited = true;
                    myqueue->push(value);
                }
                    int parentLevel=pageRankCompleteInformation->at(myqueue->front()).level;
                    int currentNodeLevel=pageRankCompleteInformation->at(value).level;
                    if(currentNodeLevel<parentLevel+1) {
                        currentNodeLevel=parentLevel+1;
                                                           #pragma omp atomic write
                        pageRankCompleteInformation->at(value).level=currentNodeLevel;
                    }




            }

        }
        myqueue->pop();

    }

}
void updateLevelforEachNode(vector<PageRank_MetaInformation> *pageRankCompleteInformation, int *p, vector<int> *nodesMarkedforUpdate)
{
   cout <<"here";
    bool change=true;
//    cout <<nodesMarkedforUpdate;
#pragma omp parallel for schedule(dynamic) num_threads(*p)
   for(int i=0;i<nodesMarkedforUpdate->size();i++)
   {  queue<int> myqueue;
       int sourceNode=nodesMarkedforUpdate->at(i);
        myqueue.push(sourceNode);
        pageRankCompleteInformation->at(sourceNode).visited=true;
        custom_build_traversal(&myqueue,pageRankCompleteInformation);

   }
//#pragma omp parallel for schedule(dynamic) num_threads(*p)
//   for(int i=0;i<pageRankCompleteInformation->size();i++)
//   {
//       if(pageRankCompleteInformation->at(i).potential_visit==true)
//       {
//           pageRankCompleteInformation->at(i).level+=pageRankCompleteInformation->at(i).visit_count;
//       }
//   }



}

#endif //PR_PR_H
