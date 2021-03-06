#include "mod.hpp"

population::population(int num,double agg,double pref){
  num_ind = num;
  for(int i=0; i<num; i++){
    double tmp = ((double) rand()/(RAND_MAX));
    double newpref = ((double) tmp > 0.2);
    double newag = 0.05 + 0.9*((double) tmp < 0.2);
    add(newag,newpref);
  }
  return;
}

population::~population(){
  indivs.clear();
  return;
}

double population::mean_agg(){
  double mean_val=0;
  int totnum = indivs.size();

  for(int i=0; i<indivs.size();i++){
    mean_val+= indivs[i].retrieve_agr()/((double) totnum);
  }

  return(mean_val);
}

double population::mean_pref(){
  double mean_val=0;
  int totnum = indivs.size();

  for(int i = 0; i < indivs.size(); i++){
    mean_val += indivs[i].retrieve_pref()/((double) totnum);
  }
  return(mean_val);
}

double population::mean_agg(int site){
  double mean_val=0;
  int totnum = 0;

  for(int i=0; i< indivs.size();i++){
    totnum += (int) (indivs[i].loc == site);
  }

  if(totnum == 0){
    return(nan(""));
  }

  for(int i = 0; i < indivs.size(); i++){
    if(indivs[i].loc == site){
      mean_val += indivs[i].retrieve_agr()/((double) totnum);
    }
  }
  return(mean_val);
}

double population::mean_pref(int site){
  double mean_val=0;
  int totnum = 0;

  for(int i=0; i< indivs.size();i++){
    totnum += (int) (indivs[i].loc == site);
  }

  if(totnum == 0){
    return(nan(""));
  }

  for(int i = 0; i < indivs.size(); i++){
    if(indivs[i].loc == site){
      mean_val += indivs[i].retrieve_pref()/((double) totnum);
    }
  }
  return(mean_val);
}

void population::add(double agg, double pref){
  wasp new_wasp(agg,pref);
  indivs.push_back(new_wasp);
}

void population::disperse(){
  for(int i =0; i<num_ind; i++){
    double tmp = ((double) rand()/RAND_MAX);
    indivs[i].loc = (int) (indivs[i].retrieve_pref() > tmp);
  }
}

void population::mutate(){
  
  std::normal_distribution<double> distribution(0.0,0.01);
  std::default_random_engine generator(rand());
  
  for(int i = 0; i < num_ind; i++){
    double tmp = ((double) rand()/(RAND_MAX));
    double tmp2 = ((double) rand()/(RAND_MAX));
    if(tmp < 0.01){
      indivs[i].mut_agr(distribution(generator));
    }

    if(tmp2 < 0.001){
      indivs[i].mut_pref(distribution(generator));
    }
  }

}

void population::reproduce(){
  for(int curr_loc = 0; curr_loc < 2; curr_loc++){
    int inds_at_loc = 0;
    int num_alive = 0;
    int ind_alive[num_ind];
  
    for(int i= 0; i < num_ind; i++){
      if(indivs[i].alive && indivs[i].loc == curr_loc){
        ind_alive[num_alive] = i;
        num_alive += 1;
      }
      inds_at_loc += (int) (indivs[i].loc == curr_loc);
    }

    if(num_alive == 0){
      if(inds_at_loc != 0){
	//return;
      
	std::cout << "Non-empty site has no alive individuals! Error and unexpected behaviour!\n";
	std::cout << inds_at_loc << "\t" << num_alive << std::endl;
	std::cout << curr_loc << std::endl;
	for(int i = 0; i < num_ind; i++){
	  if(indivs[i].loc == curr_loc){
	    std::cout << indivs[i].alive<<std::endl;
	  }
	}
	exit(1);
      }
    }
    
    if(num_alive > 0){
      for(int i = 0; i < num_ind; i++){
	if(!indivs[i].alive && indivs[i].loc == curr_loc){
	  // pick random parent
	  int parent = ind_alive[rand()%num_alive];
	  // copy info
	  //std::cout << indivs[i].retrieve_agr() << "\t" << indivs[parent].retrieve_agr() << "\t" << surv[i] << "\t" << surv[parent] << std::endl;
	  std::copy(&indivs[parent],&indivs[parent+1],&indivs[i]);
	  //std::cout <<  indivs[i].retrieve_agr() << "\t" << indivs[parent].retrieve_agr() << std::endl << std::endl;

	}
      }
    }
  }
  
  for(int i = 0; i < num_ind; i++){
    indivs[i].alive = true;
  }
  //std::cout << num_alive << std::endl;
  
}


void population::fight(int curr_loc){
  int inds[num_ind];
  int num_alive = 0;
    
  for(int i = 0; i < num_ind; i++){
    //std::cout << indivs[i].alive << "\t" << indivs[i].loc << "\t" << curr_loc << std::endl; 
    if(indivs[i].alive && indivs[i].loc == curr_loc){
      inds[num_alive] = i;
      num_alive++;
    }
  }

  //  std::cout <<"alive:\t" << num_alive << std::endl;
  if(num_alive >= 2){
    unsigned seed = rand();
    shuffle(&inds[0],&inds[num_alive],std::default_random_engine(seed));
    int i = 0;
    for(int j = 0; j < (num_alive-1); j=j+2){
      duel(inds[j],inds[j+1]);
      // fight i and i+1
    }
  }
}

void population::duel(int i,int j){
  bool fight1 = ((double) rand()/ (RAND_MAX)) < indivs[i].retrieve_agr();
  bool fight2 = ((double) rand()/ (RAND_MAX)) < indivs[j].retrieve_agr();
  double outcome = ((double) rand()/RAND_MAX);
  if(fight1 && fight2){
    indivs[i].alive = outcome > 0.5;
    indivs[j].alive = outcome < 0.5;
  }else if((fight1 && outcome < 0.8) || (fight2 && outcome > 0.8)){
    indivs[i].alive = true;
    indivs[j].alive = false;
  }else if((fight2 && outcome < 0.8) || (fight1 && outcome > 0.8)){
    indivs[j].alive = true;
    indivs[i].alive = false;
  }else{
    indivs[i].alive = true;
    indivs[j].alive = true;
  }

  //std::cout << indivs[i].alive << "\t" << indivs[j].alive << std::endl;
}

wasp::wasp(double agr,double prefa){
  agressiveness = agr;
  niche_pref = prefa;
  alive = true;
  loc= 0;
  return;
}

wasp::~wasp(){
  return;
}

void wasp::set_agr(double value){
  agressiveness = ((value > 1) ? 1 : ((value < 0) ? 0 : value));
}

void wasp::mut_agr(double value){
  set_agr(agressiveness+value);
}

void wasp::set_pref(double value){
  niche_pref = (value > 1 ? 1 : (value < 0 ? 0 : value));
}

void wasp::mut_pref(double value){
  set_pref(niche_pref + value);
}

double wasp::retrieve_agr(){
  return(agressiveness);
}

double wasp::retrieve_pref(){
  return(niche_pref);
}
