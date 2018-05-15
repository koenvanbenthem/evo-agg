#ifndef MOD_HPP_INCLUDED
#define MOD_HPP_INCLUDED

#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <assert.h>
#include <stdlib.h>
class wasp {
  double agressiveness;
  double niche_pref;
public:
  bool alive;
  int loc;
  wasp(double agr,double prefa);
  ~wasp();
  void set_agr(double);
  void mut_agr(double);
  void set_pref(double);
  void mut_pref(double);
  double get_agr();
  double get_pref();
};

class population{
public:
  population(int num,double agg,double pref);
  ~population();
  //bool *surv;
  //int *loc;
  int num_ind;
  //int surv;
  std::vector<wasp> indivs;
  void add(double agg,double pref);
  void disperse();
  void mutate();
  void reproduce();
  void fight(int);
  void duel(int,int);
  double mean_agg();
  double mean_agg(int);
  double mean_pref();
  double mean_pref(int);
};

#endif // MOD1_HPP_INCLUDED
