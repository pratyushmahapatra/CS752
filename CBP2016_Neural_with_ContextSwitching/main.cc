///////////////////////////////////////////////////////////////////////
//  Copyright 2015 Samsung Austin Semiconductor, LLC.                //
///////////////////////////////////////////////////////////////////////

//Description : Main file for CBP2016 

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <map>
using namespace std;

#include "utils.h"
//#include "bt9.h"
#include "bt9_reader.h"
//#include "predictor.cc"
#include "predictor.h"


#define COUNTER     unsigned long long
UINT64 counter[20] = {0};

PREDICTOR  *brpred = new PREDICTOR();

UINT64 numMispred[20]={0};
UINT64 cond_branch_instruction_counter[20]={0};
UINT64 uncond_branch_instruction_counter[20]={0};
bt9::BT9Reader *bt9_reader[20];
OpType opType[20];
UINT64 PC[20];
bool branchTaken[20];
UINT64 branchTarget[20];
UINT64 numIter[20] = {0};

bt9::BT9Reader bt9_reader_2("/u/s/p/spati/CS752/traces/SHORT_SERVER-9.bt9.trace.gz");
auto it_2 = bt9_reader_2.begin();
typedef decltype(it_2) branch;

void CheckHeartBeat(UINT64 numIter, UINT64 numMispred)
{
  UINT64 dotInterval=1000000;
  UINT64 lineInterval=30*dotInterval;
/*
 UINT64 d1K   =1000;
 UINT64 d10K  =10000;
 UINT64 d100K =100000;
 UINT64 d1M   =1000000; 
 UINT64 d10M  =10000000;
 UINT64 d30M  =30000000;
 UINT64 d60M  =60000000;
 UINT64 d100M =100000000;
 UINT64 d300M =300000000;
 UINT64 d600M =600000000;
 UINT64 d1B   =1000000000;
 UINT64 d10B  =10000000000;


//  if(numIter % lineInterval == 0){ //prints line every 30 million branches
//    printf("\n");
//    fflush(stdout);
//  }
  if(numIter == d1K){ //prints MPKI after 100K branches
    printf("  MPKBr_1K         \t : %10.4f",   1000.0*(double)(numMispred)/(double)(numIter));   
    fflush(stdout);
  }

  if(numIter == d10K){ //prints MPKI after 100K branches
    printf("  MPKBr_10K         \t : %10.4f",   1000.0*(double)(numMispred)/(double)(numIter));   
    fflush(stdout);
  }
  
  if(numIter == d100K){ //prints MPKI after 100K branches
    printf("  MPKBr_100K         \t : %10.4f",   1000.0*(double)(numMispred)/(double)(numIter));   
    fflush(stdout);
  }
  if(numIter == d1M){
    printf("  MPKBr_1M         \t : %10.4f",   1000.0*(double)(numMispred)/(double)(numIter)); 
    fflush(stdout);
  }

  if(numIter == d10M){ //prints MPKI after 100K branches
    printf("  MPKBr_10M         \t : %10.4f",   1000.0*(double)(numMispred)/(double)(numIter));   
    fflush(stdout);
  }

  if(numIter == d30M){ //prints MPKI after 100K branches
    printf("  MPKBr_30M         \t : %10.4f",   1000.0*(double)(numMispred)/(double)(numIter));   
    fflush(stdout);
  }

  if(numIter == d60M){ //prints MPKI after 100K branches
    printf("  MPKBr_60M         \t : %10.4f",   1000.0*(double)(numMispred)/(double)(numIter));   
    fflush(stdout);
  }

  if(numIter == d100M){ //prints MPKI after 100K branches
    printf("  MPKBr_100M         \t : %10.4f",   1000.0*(double)(numMispred)/(double)(numIter));   
    fflush(stdout);
  }
  
  if(numIter == d300M){ //prints MPKI after 100K branches
    printf("  MPKBr_300M         \t : %10.4f",   1000.0*(double)(numMispred)/(double)(numIter));   
    fflush(stdout);
  }

  if(numIter == d600M){ //prints MPKI after 100K branches
    printf("  MPKBr_600M         \t : %10.4f",   1000.0*(double)(numMispred)/(double)(numIter));   
    fflush(stdout);
  }

  if(numIter == d1B){ //prints MPKI after 100K branches
    printf("  MPKBr_1B         \t : %10.4f",   1000.0*(double)(numMispred)/(double)(numIter));   
    fflush(stdout);
  }
  
  if(numIter == d10B){ //prints MPKI after 100K branches
    printf("  MPKBr_10B         \t : %10.4f",   1000.0*(double)(numMispred)/(double)(numIter));   
    fflush(stdout);
  }
 */
}//void CheckHeartBeat

// usage: predictor <trace>

branch func(int iter, int id, branch it){
	UINT64 partialMispred=0;
	for ( ; it != bt9_reader[id]->end(); ++it) {
        CheckHeartBeat(++numIter[id], numMispred[id]); //Here numIter will be equal to number of branches read
        if((numIter[id] != 0) && (numIter[id] % 99950 == 0)){
//			printf("Iteration %d: P[%d] MPKI  \t : %10.4f \n", iter, id, 1000.0*(double)(partialMispred)/(double)(10000));
//                printf("Context Switching now! P0 mispredict rate before switching is 					\t : %10.4f \n",   1000.0*(double)(numMispred)/(double)(numIter));
            fflush(stdout);
			break;
		}
        try {
          bt9::BrClass br_class = it->getSrcNode()->brClass();
          opType[id] = OPTYPE_ERROR; 

          if ((br_class.type == bt9::BrClass::Type::UNKNOWN) && (it->getSrcNode()->brNodeIndex())) { //only fault if it isn't the first node in the graph (fake branch)
            opType[id]  = OPTYPE_ERROR; //sanity check
          }
          else if (br_class.type == bt9::BrClass::Type::RET) {
            if (br_class.conditionality == bt9::BrClass::Conditionality::CONDITIONAL)
              opType[id]  = OPTYPE_RET_COND;
            else if (br_class.conditionality == bt9::BrClass::Conditionality::UNCONDITIONAL)
              opType[id]  = OPTYPE_RET_UNCOND;
            else {
              opType[id]  = OPTYPE_ERROR;
            }
          }
          else if (br_class.directness == bt9::BrClass::Directness::INDIRECT) {
            if (br_class.type == bt9::BrClass::Type::CALL) {
              if (br_class.conditionality == bt9::BrClass::Conditionality::CONDITIONAL)
                opType[id]  = OPTYPE_CALL_INDIRECT_COND;
              else if (br_class.conditionality == bt9::BrClass::Conditionality::UNCONDITIONAL)
                opType[id]  = OPTYPE_CALL_INDIRECT_UNCOND;
              else {
                opType[id]  = OPTYPE_ERROR;
              }
            }
            else if (br_class.type == bt9::BrClass::Type::JMP) {
              if (br_class.conditionality == bt9::BrClass::Conditionality::CONDITIONAL)
                opType[id]  = OPTYPE_JMP_INDIRECT_COND;
              else if (br_class.conditionality == bt9::BrClass::Conditionality::UNCONDITIONAL)
                opType[id]  = OPTYPE_JMP_INDIRECT_UNCOND;
              else {
                opType[id]  = OPTYPE_ERROR;
              }
            }
            else {
              opType[id] = OPTYPE_ERROR;
            }
          }
          else if (br_class.directness == bt9::BrClass::Directness::DIRECT) {
            if (br_class.type == bt9::BrClass::Type::CALL) {
              if (br_class.conditionality == bt9::BrClass::Conditionality::CONDITIONAL) {
                opType[id]  = OPTYPE_CALL_DIRECT_COND;
              }
              else if (br_class.conditionality == bt9::BrClass::Conditionality::UNCONDITIONAL) {
                opType[id]  = OPTYPE_CALL_DIRECT_UNCOND;
              }
              else {
                opType[id]  = OPTYPE_ERROR;
              }
            }
            else if (br_class.type == bt9::BrClass::Type::JMP) {
              if (br_class.conditionality == bt9::BrClass::Conditionality::CONDITIONAL) {
                opType[id] = OPTYPE_JMP_DIRECT_COND;
              }
              else if (br_class.conditionality == bt9::BrClass::Conditionality::UNCONDITIONAL) {
                opType[id] = OPTYPE_JMP_DIRECT_UNCOND;
              }
              else {
                opType[id] = OPTYPE_ERROR;
              }
            }
            else {
              opType[id] = OPTYPE_ERROR;
            }
          }
          else {
            opType[id] = OPTYPE_ERROR;
          }

  
          PC[id] = it->getSrcNode()->brVirtualAddr();

          branchTaken[id] = it->getEdge()->isTakenPath();
          branchTarget[id] = it->getEdge()->brVirtualTarget();

          
/************************************************************************************************************/

          if (opType[id] == OPTYPE_ERROR) { 
            if (it->getSrcNode()->brNodeIndex()) { //only fault if it isn't the first node in the graph (fake branch)
              fprintf(stderr, "OPTYPE_ERROR\n");
              printf("OPTYPE_ERROR\n");
              exit(-1); //this should never happen, if it does please email CBP org chair.
            }
          }
          else if (br_class.conditionality == bt9::BrClass::Conditionality::CONDITIONAL) { //JD2_17_2016 call UpdatePredictor() for all branches that decode as conditional
 

            bool predDir = false;

            predDir = brpred->GetPrediction(PC[id]);
            brpred->UpdatePredictor(PC[id], opType[id], branchTaken[id], predDir, branchTarget[id]); 

            if(predDir != branchTaken[id]){
              numMispred[id]++; // update mispred stats
	      	  partialMispred++;

            }
            cond_branch_instruction_counter[id]++;


          }
          else if (br_class.conditionality == bt9::BrClass::Conditionality::UNCONDITIONAL) { // for predictors that want to track unconditional branches
            uncond_branch_instruction_counter[id]++;
            brpred->TrackOtherInst(PC[id], opType[id], branchTaken[id], branchTarget[id]);
          }
          else {
            fprintf(stderr, "CONDITIONALITY ERROR\n");
            printf("CONDITIONALITY ERROR\n");
            exit(-1); //this should never happen, if it does please email CBP org chair.
          }

/************************************************************************************************************/
        }
        catch (const std::out_of_range & ex) {
          std::cout << ex.what() << '\n';
          break;
        }
      
      }
return(it);
}


int main(int argc, char* argv[]){
  
  if (argc != 3) {
    printf("usage: %s <no. of processes> <tracelist>\n", argv[0]);
    exit(-1);
  }
  
  ///////////////////////////////////////////////
  // Init variables
  ///////////////////////////////////////////////
    
//    PREDICTOR  *brpred = new PREDICTOR();  // this instantiates the predictor code
  ///////////////////////////////////////////////
  // read each trace recrod, simulate until done
  ///////////////////////////////////////////////
    int procs= atoi(argv[1]);
    std::string filename=argv[2];
    std::string trace_path[20];
    std::string total_inst[20];
    std::string total_br_inst[20];
    UINT64 total_instruction_counter[20] ={0}; 
    UINT64 current_instruction_counter[20] = {0};
    UINT64 branch_instruction_counter[20]={0};
    std::string key1 = "total_instruction_count:";	
    std::string key2 = "branch_instruction_count:";
     
    ifstream file(filename);
    if(file.is_open())
    {
//    	printf("Able to Open tracelist");
    }
    for(int i=0; i < procs; i++){
		file >> trace_path[i];  //read the tracefile names from the tracelist;
    }

    branch it[20];
    branch resume[20];
    // bt9_reader = new (20 * sizeof(bt9::BT9Reader));
    for(int i=0; i < procs; i++){
		bt9_reader[i] = new bt9::BT9Reader(trace_path[i]);
		// cout<<*bt9_reader[i].begin()->getSrcNode()->brClass();
        bt9_reader[i]->header.getFieldValueStr(key1, total_inst[i]);
 		total_instruction_counter[i] = std::stoull(total_inst[i], nullptr, 0);	
        bt9_reader[i]->header.getFieldValueStr(key2, total_br_inst[i]);	
        branch_instruction_counter[i] = std::stoull(total_br_inst[i], nullptr, 0);	
		cond_branch_instruction_counter[i]=0;
		uncond_branch_instruction_counter[i]=0;
		it[i] = bt9_reader[i]->begin();
		resume[i]=it[i];
	}
    
    int min=branch_instruction_counter[0];
    for(int j=1; j<procs; j++){
    	if(branch_instruction_counter[j] < min){
    		min=branch_instruction_counter[j];
    	}
    }
    // std::cout<<resume[p]->getSrcNode->;
    int iterations=min / 99950;
    for(int k=0; k< iterations; k++){
    	for(int p=0;p < procs; p++){
    		resume[p]=func(k, p, resume[p]);
    	}
    }
  ///////////////////////////////////////////////
  // read each trace record, simulate until done
  ///////////////////////////////////////////////

    ///////////////////////////////////////////
    //print_stats
    ///////////////////////////////////////////

    //NOTE: competitors are judged solely on MISPRED_PER_1K_INST. The additional stats are just for tuning your predictors.
	printf(" Process No., Trace, NUM INSTRUCTION,  MISPRED_PER_1K_COND_BR \n");
for(int i=0; i < procs; i++){
   //   printf("  TRACE \t : %s" , trace_path[i].c_str()); 
     // printf("  NUM_INSTRUCTIONS            \t : %10llu",   total_instruction_counter[i]);
    //  printf("  NUM_BR                      \t : %10llu",   branch_instruction_counter[i]-1); //JD2_2_2016 NOTE there is a dummy branch at the beginning of the trace...
   //   printf("  NUM_UNCOND_BR               \t : %10llu",   uncond_branch_instruction_counter[i]);
  //    printf("  NUM_CONDITIONAL_BR          \t : %10llu",   cond_branch_instruction_counter[i]);
//      printf("  NUM_MISPREDICTIONS          \t : %10llu",   numMispred[i]);
      printf(" %d, %s, %10llu, %10.4f", i, trace_path[i].c_str(), numIter[i], 1000.0*(double)(numMispred[i])/(double)(cond_branch_instruction_counter[i]));
      // printf("   MISPRED_PER_1K_INST         \t : %10.4f",   1000.0*(double)(numMispred[i])/(double)(branch_instruction_counter[i]));
      printf("\n");
      }
}



