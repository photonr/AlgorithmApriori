#include "stdafx.h"
#include "AssociationRules.h"

AssociationRules::AssociationRules(const double support, std::ifstream & file) :
supportRatio(support),
countTransactions(0),
dataFile(file)
{
}


AssociationRules::~AssociationRules()
{
}

// Run calc rules
void AssociationRules::Work()
{
  Parsing();
  EraseWeakSupportProducts();
  // generate rules from 2size to max size
  const size_t countProducts = ruleTransaction.size();
  for (auto i = 2; i <= countProducts; ++i)
  {
    auto newCandidates = GenerateRules(ruleTransaction, i);
    ruleTransaction.insert(newCandidates.begin(), newCandidates.end());
  }
}
// parsing file with products. Save data in map as <rule(product), transaction array >
void AssociationRules::Parsing()
{
  std::string line;
  size_t idTransaction = 0;
  while (dataFile)
  {
    std::getline(dataFile, line);
    if (!line.empty())
    {
      for (size_t i = 0; i < line.size(); ++i)
      {
        if (line[i] != '0')
          ruleTransaction[{i}].push_back(idTransaction);
      }
      ++idTransaction;
    }
  }
  countTransactions = idTransaction;
}

// Delete elements with not enough support
void AssociationRules::EraseWeakSupportProducts()
{
  for (auto it = ruleTransaction.begin(); it != ruleTransaction.end();)
  {
    if (IsSupportSatisfy(it->second))
    {
      ++it;
    }
    else
    {
      it = ruleTransaction.erase(it);
    }
  }
}

// Generate rules with size - 'sizeRule'
// getting rulles map with sufficient support and size of new rule. Return map with rules.
MapRuleTransaction AssociationRules::GenerateRules(MapRuleTransaction& ruleTransaction, size_t sizeRule)
{
  if (sizeRule < 2)
  {
    throw(std::runtime_error("GenerateRules: Error, generate candidate only for >= 2 size rule; "  + std::to_string(sizeRule)));
  }
  MapRuleTransaction newRules;

  // find the first element wich rule consist of (sizeRule-1) elements
  auto it = std::find_if(ruleTransaction.begin(), ruleTransaction.end(), 
            [sizeRule](const std::pair<Rule, Transactions>& x) {return x.first.size() == sizeRule-1;});

  // generate rules with 'sizeRule' elements. 
  // Unite all rules with the same base with each other (they are all with required support in tree). Exp: ab->ac = abc 
  for (; it !=  ruleTransaction.end(); ++it)
  {
    auto it2 = it;
    for (++it2; it2 != ruleTransaction.end(); ++it2)
    {
      if (it->first.size() < sizeRule-1 || it2->first.size() < sizeRule-1)
      {
        throw(std::runtime_error("GenerateRules: Error, map is wrong sorting. size of candidates different. Rule: " + std::to_string(sizeRule)));
      }
      // find rule with the same base. If we build rule with 'sizeRule', need find 'sizeRule-2' base in 'sizeRule-1' rule.
      if (std::includes(it->first.begin(), it->first.begin() + (sizeRule-2), it2->first.begin(), it2->first.begin() + (sizeRule-2)))
      {
        // unite it and it2 rules, and check support of new rule.
        auto transactions = decltype(it->second){};
        std::set_intersection(it->second.begin(), it->second.end(), it2->second.begin(), it2->second.end(), std::back_inserter(transactions));
        if (IsSupportSatisfy(transactions))
        { // if support is ok, make new rule and save
          auto rule = decltype(it->first){};
          std::set_union(it->first.begin(), it->first.end(), it2->first.begin(), it2->first.end(), std::back_inserter(rule));
          newRules.insert(std::make_pair(rule, transactions));
        }
      }
      // if it and it2 don't have same base -> rest elements don't have same base, by design. 
      else
      {
        continue;
      }
    }
  }
  return newRules;
}

bool AssociationRules::IsSupportSatisfy(const Transactions& transactions) const
{
  return CalcSupport(transactions.size()) > AssociationRules::supportRatio;
}

double AssociationRules::CalcSupport(const size_t countRuleTransactions) const
{
  return static_cast<double>(countRuleTransactions) / countTransactions;
}