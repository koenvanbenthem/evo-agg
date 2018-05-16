#include "svgplot.hpp"
#include <iostream>
#include <fstream>
#include <stdlib.h>

plot::plot(std::string filename){
  num_panels = 3;
  file.open(filename);
  write_header();
  add_panel(0.0,1e5,0.0,1.0,1.5,2.0,8.0,17.0,"Overall");
  add_panel(0.0,1e5,0.0,1.0,11.5,2.0,8.0,17.0,"Patch 1");
  add_panel(0.0,1e5,0.0,1.0,21.5,2.0,8.0,17.0,"Patch 2");
}

plot::~plot(){
  write_tail();
  file.close();
}

void plot::print(){
  std::cout << "noooo";
  
}

void plot::write_header(){
  file << "<?xml version=\"1.0\" standalone=\"yes\"?>\n";
  file << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
  file << "<svg width=\"32cm\" height=\"21cm\" fill=\"white\" version=\"1.1\">\n";
  file << "<desc> automatically generated svg file </desc>\n";
  file << "<rect x=\"0cm\" y=\"0cm\" width=\"32cm\" height=\"21cm\" fill=\"white\" />\n";

  file << "<text font-family=\"Verdana\" font-size=\"20\" fill=\"black\" >\n";
  file << "<textPath xlink:href=\"#ylab\">\n";
  file << "Allele frequency\n";
  file << " </textPath>\n </text>\n";
  
}

void plot::write_panel_header(double xmin, double xmax, double ymin, double ymax, double c_xmin, double c_ymin, double c_width, double c_height, std::string title){
    file << "<rect x=\""<<c_xmin<<"cm\" y=\""<<c_ymin<<"cm\" width=\""<<c_width << "cm\" height=\""<<c_height<<"cm\" stroke=\"black\"/>\n";
    file << "<text x=\""<<c_xmin<<"cm\" y=\""<<c_ymin - 0.6 <<"cm\" font-family=\"Verdana\" fill=\"black\" font-size=\"20\">"<<title<<"</text>\n";
    file << "<text x=\""<<c_xmin- 0.1<<"cm\" y=\""<<c_ymin +c_height + 0.6 <<"cm\" font-family=\"Verdana\" fill=\"black\" font-size=\"20\">"<<xmin<<"</text>\n";
    file << "<text x=\""<<c_xmin+c_width - 0.5<<"cm\" y=\""<<c_ymin + c_height +  0.6 <<"cm\" font-family=\"Verdana\" fill=\"black\" font-size=\"20\">"<<xmax<<"</text>\n";
    file << "<text x=\""<<c_xmin- 0.5 <<"cm\" y=\""<<c_ymin + c_height <<"cm\" font-family=\"Verdana\" fill=\"black\" font-size=\"20\">"<<ymin<<"</text>\n";
    file << "<text x=\""<<c_xmin- 0.5 <<"cm\" y=\""<<c_ymin + 0.2 <<"cm\" font-family=\"Verdana\" fill=\"black\" font-size=\"20\">"<<ymax<<"</text>\n";
}

void plot::add_panel(double xmin, double xmax, double ymin, double ymax, double c_xmin, double c_ymin, double c_width, double c_height,std::string title){
  panel new_panel;
  new_panel.content_lims[0] = xmin;
  new_panel.content_lims[1] = xmax;
  new_panel.content_lims[2] = ymin;
  new_panel.content_lims[3] = ymax;
  new_panel.canvas_lims[0] = c_xmin*pixels_per_cm;
  new_panel.canvas_lims[1] = (c_xmin + c_width)*pixels_per_cm;
  new_panel.canvas_lims[2] = c_ymin*pixels_per_cm;
  new_panel.canvas_lims[3] = (c_ymin + c_height)*pixels_per_cm;
  panels.push_back(new_panel);
  write_panel_header(xmin,xmax,ymin, ymax,c_xmin,c_ymin,c_width,c_height,title);
}

void plot::add_line_to_panel(double* data, int length,int panel){
  panels[panel].add_line(data, length, file);
}

void plot::write_tail(){
file << "\n\n</svg>";
}

panel::panel(){
  num = 3;
  content_lims = (double *) std::calloc(4, sizeof(double));
  canvas_lims = (double *) calloc(4,sizeof(double));
}

panel::~panel(){
  //  free(content_lims);
//free(canvas_lims);
}

void panel::add_line(double* data, int length, std::ofstream& file){
  file << "<path d =\"M" << dat_to_x(0) <<" " << dat_to_y(data[0]);
  for(int i = 1; i < (length); i++){
    file << " L" << dat_to_x(i) << " " << dat_to_y(data[0]);
  }
  file << " stroke=\"black\" stroke-width=\"3\" />\n\n";
}

double panel::dat_to_x(double x){
  double temp = canvas_lims[0] + (canvas_lims[1]-canvas_lims[0])*(x - content_lims[0])/(content_lims[1] - content_lims[0]);

  return(temp);
}

double panel::dat_to_y(double y){
  double temp = canvas_lims[2] + (canvas_lims[3]-canvas_lims[2])*(y - content_lims[2])/(content_lims[3] - content_lims[2]);
  return(temp);
}
