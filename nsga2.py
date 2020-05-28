import random
import numpy

import deap
import deap.gp
import deap.benchmarks
from deap import base
from deap import creator
from deap import tools
from deap import algorithms
import multiprocessing
import time

random.seed(time.time)
POP_SIZE =  100
OFFSPRING_SIZE = 50

NGEN = 70
ALPHA = POP_SIZE
MU = OFFSPRING_SIZE
LAMBDA = OFFSPRING_SIZE
CXPB = 0.7
MUTPB = 0.3
ETA = 10.0

SELECTOR = "NSGA2"

IND_SIZE =17
LOWER, UPPER = [0, 0, 0, 0,0, 0, 0, 0, 0, -20, 1, -20, .5,-20, .5, 0.5, 0.5], [1.0, 5, 3.0, 3.0, 100, 1.0, 1.0, 1000, 500, 20, 2, 20, 2, 20, 2, 2, 2] 

creator.create("Fitness", base.Fitness, weights=[-100.0, -100.0, -1.0, -1.0, -1.0, -100.0])
creator.create("Individual", list, fitness=creator.Fitness)


def uniform(lower_list, upper_list, dimensions):
    """Fill array """

    if hasattr(lower_list, '__iter__'):
        return [random.uniform(lower, upper) for lower, upper in
                zip(lower_list, upper_list)]
    else:
        return [random.uniform(lower_list, upper_list)
                for _ in range(dimensions)]

toolbox = base.Toolbox()
toolbox.register("uniformparams", uniform, LOWER, UPPER, IND_SIZE)
toolbox.register(
    "Individual",
    tools.initIterate,
    creator.Individual,
    toolbox.uniformparams)
toolbox.register("population", tools.initRepeat, list, toolbox.Individual)


import deap_LP_eval2
toolbox.register("evaluate",  deap_LP_eval2.evaluate)

toolbox.register(
    "mate",
    deap.tools.cxSimulatedBinaryBounded,
    eta=ETA,
    low=LOWER,
    up=UPPER)
toolbox.register("mutate", deap.tools.mutPolynomialBounded, eta=ETA,
                 low=LOWER, up=UPPER, indpb=0.1)

toolbox.register("variate", deap.algorithms.varAnd)

toolbox.register(
    "select",
    tools.selNSGA2)

pop = toolbox.population(n=MU)

pool = multiprocessing.Pool(4)
toolbox.register("map", pool.map)
hof = tools.HallOfFame(100)


first_stats = tools.Statistics(key=lambda ind: ind.fitness.values[0])
second_stats = tools.Statistics(key=lambda ind: ind.fitness.values[1])
third_stats  = tools.Statistics(key=lambda ind: ind.fitness.values[2])
fourth_stats = tools.Statistics(key=lambda ind: ind.fitness.values[3])
fifth_stats = tools.Statistics(key=lambda ind: ind.fitness.values[4])
sixth_stats = tools.Statistics(key=lambda ind: ind.fitness.values[5])
stats = tools.MultiStatistics(obj1=first_stats, obj2=second_stats, obj3=third_stats, obj4=fourth_stats \
 , obj5=fifth_stats, obj6=sixth_stats)
stats.register("min", numpy.min, axis=0)

if __name__ == '__main__':
    pop, logbook = algorithms.eaMuPlusLambda(
        pop,
        toolbox,
        MU,
        LAMBDA,
        CXPB,
        MUTPB,
        NGEN,
        stats,
        halloffame=hof)


    # save population to a file
    numpy.savetxt('final_pop.dat', pop)
    numpy.savetxt('hallOfFame.dat', hof)

