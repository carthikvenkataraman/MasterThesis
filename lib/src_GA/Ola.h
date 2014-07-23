#define DRIVELAB_GENETICALGORITHM_HPP

#include <memory>
#include <random>
#include <utility>
#include <vector>

namespace drivelab {

typedef std::vector<float> Fitnesses;
typedef std::vector<double> Individual;
typedef std::vector<Individual> Population;

class Problem;

class GeneticAlgorithm {
  public:
    GeneticAlgorithm(std::shared_ptr<Problem const>, int, int, int, int, float,
        float, float);
    virtual ~GeneticAlgorithm();
    float GetBestFitness() const;
    Individual GetBestIndividual() const;
    int GetGenerationIndex() const;
    std::vector<Individual> GetHistoricIndividuals() const;
    std::vector<Individual> GetHistoricIndividuals(int) const;
    void NextGeneration(int);
    void Test();

  private:
    GeneticAlgorithm(GeneticAlgorithm const &);
    GeneticAlgorithm &operator=(GeneticAlgorithm const &);
    std::pair<Individual, Individual> CrossIndividuals(Individual const &,
        Individual const &);
    Fitnesses EvaluatePopulation(Population const &, int);
    Population GeneratePopulation();
    int GetRandomInteger(int, int);
    double GetRandomCreep();
    double GetRandomDouble();
    float GetRandomFloat();
    Individual MutateIndividual(Individual const &);
    Individual SelectTournament(Population const &, Fitnesses const &);

    std::default_random_engine m_generator;
    std::shared_ptr<Problem const> m_problem;
    std::vector<Individual> m_historic_individuals; 
    Fitnesses m_fitnesses;
    Individual m_best_individual;
    Population m_population;
    float m_best_fitness;
    int m_generation_index;
    float const m_prob_crossover;
    float const m_prob_mutation;
    float const m_prob_select_tournament;
    int const m_elite_size;
    int const m_population_size;
    int const m_tournament_size;
};

}

#endif
