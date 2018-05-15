#ifndef SVGPLOT_HPP_INCLUDED
#define SVGPLOT_HPP_INCLUDED

#include<string>
#include<fstream>
#include <vector>

const double pixels_per_cm = 35.43307;

class panel{
public:
  double* content_lims;
  double* canvas_lims;
  int num;
  panel();
  ~panel();
};
  
class plot{
public:
  int num_panels;
  std::vector<panel> panels;
  void add_panel(double xmin, double xmax, double ymin, double ymax, double c_xmin, double c_ymin, double c_width, double c_height, std::string title);
  void write_panel_header(double xmin, double xmax, double ymin, double ymax, double c_xmin, double c_ymin, double c_width, double c_height, std::string title);
  void print();
  std::ofstream file;
  void write_header();
  void write_tail();
  plot(std::string filename);
  ~plot();
};

#endif
