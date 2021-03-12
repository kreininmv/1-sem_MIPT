#ifndef DIFFH
#define DIFFH
/* FILE NAME   : diff.h
 * PURPOSE     : Function module.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 04.12.2020.
 * NOTE        : None.
 */


#define TEX_INTRODUCTION "\\documentclass[a4paper,12pt]{article}\n"                  \
                         "\\usepackage[T2A]{fontenc}\n"                              \
                         "\\usepackage[utf8]{inputenc}\n"                            \
                         "\\usepackage[english, russian]{babel}\n"                   \
                         "\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools}\n" \
                         "\\usepackage{wasysym}\n"                                   \
                         "\\usepackage{pgfplots}"                                    \
                         "\\pgfplotsset{compat = 1.9}"                               \
                         "\\usepackage{graphicx}"                                    \
                         "\\usepackage[unicode, pdftex]{hyperref}"                   \
                         "\\usepackage[normalem]{ulem}"                              \
                         "\\graphicspath{{pictures/}}"                               \
                         "\\DeclareGraphicsExtensions{.pdf,.png,.jpg}"               \
                         "\\author{Крейнин Матвей}\n"                                \
                         "\\title{T06 DIFF}\n"                                       \
                         "\\date{\\today}\n"                                         \
                         "\\begin{document}\n"                                       \
                         "\\maketitle\n"                                             \
                         "\\thispagestyle{empty}\n"                                  \
                         "\\newpage\n"                                               \
                         "\\setcounter{page}{1}\n"                                   \
                         "\\newcommand{\\newsin}[2]{\\sin^{#2}{#1}}\n"               \
                         "\\newcommand{\\newcos}[2]{\\cos^{#2}{#1}}\n"               \
                         "\\newcommand{\\newtg}[2]{\\tg^{#2}{#1}}\n"                 \
                         "\\newcommand{\\newctg}[2]{\\ctg^{#2}{#1}}\n"               \
                         "\\newcommand{\\newsqrt}[2]{\\sqrt^{#2}{#1}}\n"             \
                         "\\newcommand{\\newexp}[2]{\\exp^{#2}{#1}}\n"               \
                         "\\newcommand{\\newln}[2]{\\ln{#2}{#1}}\n"                  \
                         "\\begin{center}\n"

#include "parser.h"

namespace mk5 {

  class differenciator {
  private:
    tree mytree;
    parser myparser;

    double eval(const vertex* node);

    int write_vertex_dump(vertex* node, const int num_node, FILE* file);

    int write_vertex_tex(vertex* node, const int num_node, FILE* file, const int type_bracket);
 
    int simplify_vertex(vertex* node, const int num_node);

    int find_var_vertex(vertex* node, int num_var);

    int find_priority_lowest(vertex* node);

    int get_name_diff(double value, type_vertex type, char* str);

    vertex* make_taylor(int degree, int num_var_diff);

    int make_graphic(FILE* file, int num_var_diff);

    vertex* add_polynom(vertex* root, double value, int degree, int num_var_diff);
  public:
    int dump_graph(void);
    
    int dump_tex(void);

    vertex* diff(vertex* node, int &num_node, int var_diff);

    vertex* write_article(FILE *file, vertex* node, int &num_node, int var_diff, const int type_bracket);

    int love_math(char *expr, char *var);

    int convert_debug(char* expr);

    differenciator() = default;
    ~differenciator() {;}

    differenciator  operator= (const differenciator & copy) = delete;
    differenciator(const differenciator & copy) = delete;

  };
}

#endif
