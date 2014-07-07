#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "numbbo.h"

#include "numbbo_problem.c"

/* Schaffers F7 function, transformations not implemented for the moment  */

static void f_schaffers_evaluate(numbbo_problem_t *self, double *x, double *y) {
    size_t i, k;
    assert(self->number_of_objectives == 1);

    /* Computing f_pen */
    double f_pen = 0, diff, f_opt = 0; /* f_opt may need to be changed*/
    static const double condition = 1.0e2;
    for (i = 0; i < self->number_of_parameters; ++i){
    	diff = fabs(x[i]) - 5;
    	if (diff > 0){
    		f_pen += diff * diff;
    	}
    }

    /* Computation core */
    y[0] = 0.0;
    double tmp;
    for (i = 0; i < self->number_of_parameters - 1; ++i) {
    	tmp = x[i] * x[i] + x[i + 1] * x[i + 1];
    	y[0] += pow(tmp, 0.25) * (1 + pow(sin(50. * pow(tmp, 0.1)), 2.));

    }
    y[0] = pow((1./((double)(self->number_of_parameters - 1))) * y[0], 2.) + 10 * f_pen + f_opt;
}

static numbbo_problem_t *schaffers_problem(const size_t number_of_parameters) {
    size_t i, problem_id_length;
    numbbo_problem_t *problem = numbbo_allocate_problem(number_of_parameters, 1, 0);
    problem->problem_name = numbbo_strdup("schaffers function");
    /* Construct a meaningful problem id */
    problem_id_length = snprintf(NULL, 0,
                                 "%s_%02i", "schaffers", (int)number_of_parameters);
    problem->problem_id = numbbo_allocate_memory(problem_id_length + 1);
    snprintf(problem->problem_id, problem_id_length + 1,
             "%s_%02d", "schaffers", (int)number_of_parameters);

    problem->number_of_parameters = number_of_parameters;
    problem->number_of_objectives = 1;
    problem->number_of_constraints = 0;
    problem->evaluate_function = f_schaffers_evaluate;
    for (i = 0; i < number_of_parameters; ++i) {
        problem->lower_bounds[i] = -5.0;
        problem->upper_bounds[i] = 5.0;
        problem->best_parameter[i] = 0.0;
    }
    /* Calculate best parameter value */
    f_schaffers_evaluate(problem, problem->best_parameter, problem->best_value);
    return problem;
}


