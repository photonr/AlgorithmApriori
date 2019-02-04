#include "stdafx.h"
#include "PresenterRules.h"

PresenterRules::PresenterRules()
{
}

PresenterRules::~PresenterRules()
{
}

// output in stringstream rules information about rules as: number {rule}; support = ruleSupport
std::ostringstream PresenterRules::GetRules(const AssociationRules& parsing, const TypeSorting typeSort, const size_t countRules)
{
  std::ostringstream os;
  os << "Max print rules = " << countRules << std::endl;
  os << "Support value = " << parsing.supportRatio << std::endl;
  // find in map first element with rule of 2 products. All rulles in map at this stage with needed support
  auto itM = std::find_if(parsing.ruleTransaction.begin(), parsing.ruleTransaction.end(),
           [](const std::pair<Rule, Transactions>& x) {return x.first.size() > 1; });

  // make vector rules <rule, support> with rules >= 2 products
  std::vector<std::pair<Rule, double>> rules;
  for (; itM != parsing.ruleTransaction.end(); ++itM)
  {
    rules.emplace_back(itM->first, parsing.CalcSupport(itM->second.size()));
  }

  PresenterRules::SortRules(rules, typeSort);
  // 
  for (auto it = rules.begin(); it != rules.end() && std::distance(rules.begin(), it) < countRules; ++it)
  {
    os << std::distance(rules.begin(), it) << ". {";
    for (auto & val : it->first)
    {
      os << val << ", ";
    }
    os << "}; " << "support = " << it->second << std::endl;
  }
  return os;
}

void PresenterRules::SortRules(std::vector<std::pair<Rule, double>>& rules, const TypeSorting typeSort)
{
  switch (typeSort)
  {
  case TypeSorting::IncreaseSupport:
    std::sort(rules.begin(), rules.end(),
      [](const std::pair<Rule, double> & a, std::pair<Rule, double> & b) {return a.second < b.second; });
    break;
  case TypeSorting::DecreaseSupport:
    std::sort(rules.begin(), rules.end(),
      [](const std::pair<Rule, double> & a, std::pair<Rule, double> & b) {return a.second > b.second; });
    break;
  case TypeSorting::None:
    break;
  }
}