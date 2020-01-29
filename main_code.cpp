

#include "SP_structure.hpp"
#include "readin_data.hpp"
#include "verify.hpp"
#include "delete_ops1.hpp"
#include "insert_ops1.hpp"
#include "printout_others.hpp"


using namespace std;

int main(int argc, char *argv[])
{       
	clock_t q, q1, q2,t;
	
    
    //Check if valid input is given
    if ( argc < 3) { cout << "INPUT ERROR:: 5 inputs required. First: filename \n Second: file with component id \n  Third: Set of changed edges \n Fourth: Number of nodes \n Fifth: Number of threads \n";}
    //Check to see if file opening succeeded
    ifstream the_file1 ( argv[1] ); if (!the_file1.is_open() ) { cout<<"INPUT ERROR:: Could not open file\n";}
    ifstream the_file2 ( argv[2] ); if (!the_file2.is_open() ) { cout<<"INPUT ERROR:: Could not open file\n";}
    ifstream the_file3 ( argv[3] ); if (!the_file3.is_open() ) { cout<<"INPUT ERROR:: Could not open file\n";}
  int p = atoi(argv[5]);  //total number of threads per core
  int nodes=atoi(argv[4]);//total number of nodes
  
    /**=============================================**/
    /*** Reading in Results at Time Step 0 ***/
    /***============================================**/
    //Information about SCC of each vertex

    SCC_Comp CI;
    vector<SCC_Comp> C_Info;
    C_Info.resize(nodes,CI);
    
    SCC_Network X;
    
    q=clock();
    VertexProp VP;
    X.resize(nodes, VP);
    readin_SCC(argv[2],&X,&C_Info);
    q=clock()-q;
    
    cout << "Total Time for Reading SCC "<< ((float)q)/CLOCKS_PER_SEC <<"\n";
    /**=============================================**/
    
    /**=============================================**/
    /*** Reading in Network at Time Step 0 ***/
    /***============================================**/
        //Obtain the list of edges.
        q=clock();
         readin_network(&X,argv[1]);
         //Update batch size in parallel
         batch_update(&X,p);
        q=clock()-q;
    
        cout << "Total Time for Reading Network"<< ((float)q)/CLOCKS_PER_SEC <<"\n";
  /**=============================================**/
    
  
    //The following modules will iterate over each batch
      int b=1; //Batch number
    /**=============================================**/
    /*** Reading in Changed Edges of Batch b***/
    /***============================================**/
    q=clock();
    
    readin_changes(argv[3], b,&X);
    //Update batch size in parallel
    batch_update(&X,p);
    
    q=clock()-q;
    cout << "Total Time for Reading Changed Edges "<< ((float)q)/CLOCKS_PER_SEC <<"\n";
   /**=============================================**/
    
    
   /* print_CInfo(&C_Info);
    print_Network(&X);*/
    
    /**=============================================**/
    /*** Process Deletes ***/
    /***============================================**/
   printf("Start delete \n");
   q=clock();
    //Step 1: Find components that are affected by deletion
    find_changed_componentsDel(&X,b,p);
    
  //Step 2. Find the new components
     // Parallel over vertices in each component do fw-bw
    //This is equal to doing fw-bw in parallel for each pivot
   change_componentsDel(&X, &C_Info,b,p);
    
    //Step 3. Get singletons
    get_singletons(&X, &C_Info,p);
    
    //Step 4. Check whether the singletons form a component
    singleton_componentsDel(&X, &C_Info,b,p);
    
    //Step 5. Update information about the components
    //Create new C_Info to store the components
   vector<SCC_Comp> C_Info1;
   C_Info1.resize(nodes,CI);
   update_cinfoDel(&X,&C_Info, &C_Info1,p);
   C_Info=C_Info1;
   C_Info1.clear();
    
    q=clock()-q;
    cout << "Total Time for Processing Deletes "<< ((float)q)/CLOCKS_PER_SEC <<"\n";
    
    printf("After delete \n");
    print_component(&X, &C_Info);
   // print_Network(&X);
    /**=============================================**/
    /*** Process Inserts ***/
    /***============================================**/
    printf("Start insert \n");
    q=clock();
    
    //Step 1: Find components that are affected by insertion
    find_changed_componentsIns(&X, &C_Info,b,p);
    printf("done \n");
    
    //Step 2 Get Cmap to find the number of components
    vector<int> Cmap;
    map_components(&C_Info, &Cmap,p);
    printf("done1 \n");
    print_vector(Cmap);
    
    //Step 3. Find components that joined
    // Parallel over vertices in each component do fw-bw
    //This is equal to doing fw-bw in parallel for each pivot
    change_componentsIns(&X, &C_Info, &Cmap,b,p);
    printf("done2 \n");
    
    //Step 4. Update information about the components
    update_cinfoIns(&X,&C_Info,p);
    printf("done3 \n");
    
    q=clock()-q;
    cout << "Total Time for Processing Inserts: "<< ((float)q)/CLOCKS_PER_SEC <<"\n";
 
    print_component(&X, &C_Info);
    printf("After insert \n");
    //print_CInfo(&C_Info);
    

	return 0;
}//end of main
	
