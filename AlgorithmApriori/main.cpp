// AlgorithmApriori.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "AssociationRules.h"
#include "PresenterRules.h"

const size_t countInputArgs = 4; // name, support, sortingType, countRulesForPrint
void SetRulesConfig(int argc, char *argv[], double& support, TypeSorting &sortingType, size_t & countRulesPrint);

int main(int argc, char *argv[])
{
  try
  {
    std::ifstream file("association_rules.data", std::ios::in);
    if (!file.is_open())
    {
      throw std::runtime_error("Could not open file association_rules");
    }

		double support;
		TypeSorting sortingType;
		size_t countRulesPrint;
		SetRulesConfig(argc, argv, support, sortingType, countRulesPrint);
      
		auto timeStart = std::chrono::system_clock::now();

    AssociationRules associationRules(support, file);
    associationRules.Work();
		// get output result
    auto os = PresenterRules::GetRules(associationRules, sortingType, countRulesPrint);
    std::cout << os.str();

    auto timeEnd = std::chrono::system_clock::now();
    std::cout << "time = " << std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeStart).count() << std::endl;
    getchar();
  }
  catch (const std::exception & e)
  {
    std::cerr << e.what();
    getchar();
    return 1;
  }
  return 0;
}

// Parsing input from argv
void SetRulesConfig(int argc, char *argv[], double& support, TypeSorting & sortingType, size_t & countRulesPrint)
{
	if (argc == countInputArgs)
	{
		support = std::stod(argv[1]);
		countRulesPrint = std::stoi(argv[3]);
		int sorting = std::stoi(argv[2]);
		if (sorting == 1)
			sortingType = TypeSorting::DecreaseSupport;
		else if (sorting == -1)
			sortingType = TypeSorting::DecreaseSupport;
		else
			sortingType = TypeSorting::None;
	}
	else
	{
		throw std::runtime_error("Not correct count of arguments in starting input");
	}
}