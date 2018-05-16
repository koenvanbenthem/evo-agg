#include <iostream>
#include <fstream>
#include "mod.hpp"
#include "svgplot.hpp"
//#include <plstream>
//#include <boost>
using namespace std;

// input values :: output_filename N N_fights replicates start_ag 
int main(int argc, char **argv)
{
  int N, N_t,N_fights,replicates;
  double init_ag;
  string base_file, file_ag_name, file_ag1_name, file_ag2_name, file_pref_name, file_pref1_name, file_pref2_name;
  ofstream file_ag,file_ag1,file_ag2,file_pref,file_pref1,file_pref2;
  if(argc < 7){
    cout << "not enough arguments passed! Default values will be used!" <<endl;
    //    exit(1);
    N = 100;
    N_t = 1e5;
    N_fights = 1;
    replicates = 2;
    init_ag = 0.5;
    base_file = "testfile";
  } else {
    N = atoi(argv[2]);
    N_t = atoi(argv[6]);
    N_fights = atoi(argv[3]);
    replicates = atoi(argv[4]);
    init_ag = atof(argv[5]);
    base_file = argv[1];
  }
  cout << SIZE_MAX << "\t"  << sizeof(double) << endl;
  double overall_agr[N_t];
  double patch1_agr[N_t];
  double patch2_agr[N_t];
  double overall_pref[N_t];
  double patch1_pref[N_t];
  double patch2_pref[N_t];
  file_ag_name = base_file;
  file_ag1_name = base_file;
  file_ag2_name = base_file;
  file_pref_name = base_file;
  file_pref1_name = base_file;
  file_pref2_name = base_file;
  
  file_ag_name.append(".ag");
  file_ag1_name.append(".ag1");
  file_ag2_name.append(".ag2");
  file_pref_name.append(".pr");
  file_pref1_name.append(".pr1");
  file_pref2_name.append(".pr2");
  
  file_ag.open(file_ag_name);
  file_ag1.open(file_ag1_name);
  file_ag2.open(file_ag2_name);
  file_pref.open(file_pref_name);
  file_pref1.open(file_pref1_name);
  file_pref2.open(file_pref2_name);

  srand(time(NULL));
  
  for(int k =0; k<replicates; k++){
    population full_pop(N,init_ag,0.5);
  
    for(int i=0; i<N_t; i++){
      full_pop.disperse();
      for(int j= 0; j < N_fights; j++){
	full_pop.fight(0);
	full_pop.fight(1);
      }
      //cout << endl<<endl;
      full_pop.reproduce();
      file_ag << full_pop.mean_agg();
      file_ag1 << full_pop.mean_agg(0);
      file_ag2 << full_pop.mean_agg(1);
      file_pref << full_pop.mean_pref();
      file_pref1 << full_pop.mean_pref(0);
      file_pref2 << full_pop.mean_pref(1);
      
      overall_agr[i] = full_pop.mean_agg();
      patch1_agr[i] = full_pop.mean_agg(0);
      patch2_agr[i] = full_pop.mean_agg(1);

      overall_pref[i] = full_pop.mean_pref();
      patch1_pref[i] = full_pop.mean_pref(0);
      patch2_pref[i] = full_pop.mean_pref(1);
      
      if(i < N_t -1){
	file_ag << ",";
	file_ag1 << ",";
	file_ag2 << ",";
	file_pref << ",";
	file_pref1 << ",";
	file_pref2 << ",";
      }
      
      full_pop.mutate(); 
    }
    
    string plotfile = base_file;
    plotfile.append(std::to_string(k));
    plotfile.append(".svg");
    plot targetplot(plotfile,0,N_t,0,1);
    targetplot.add_line_to_panel(overall_agr,N_t,0,"black");  
    targetplot.add_line_to_panel(patch1_agr,N_t,1,"black");
    targetplot.add_line_to_panel(patch2_agr,N_t,2,"black");
  
    targetplot.add_line_to_panel(overall_pref,N_t,0,"red");  
    targetplot.add_line_to_panel(patch1_pref,N_t,1,"red");
    targetplot.add_line_to_panel(patch2_pref,N_t,2,"red");
    
    file_ag << endl;
    file_ag1 << endl;
    file_ag2 << endl;
    file_pref << endl;
    file_pref1 << endl;
    file_pref2 << endl;
    }
    file_ag.close();
    file_ag1.close();
    file_ag2.close();
    file_pref.close();
    file_pref1.close();
    file_pref2.close();
    return 0;
}
