#pragma once

// Transactions - for save all id transactions of rule
using Transactions = std::vector<uint32_t>;
// rule, value - product number
using Rule = std::vector<uint32_t>;
// Comaparator for map<Rule, Transactions>, priority by size rule and then values
struct RuleComparator {
  bool operator()(const Rule& a, const Rule& b) const
  {
    if (a.size() != b.size())
    {
      return a.size() < b.size();
    }
    else
    {
      for (auto i = 0; i < a.size(); ++i)
      {
        if (a[i] != b[i])
        {
          return a[i] < b[i];
        }
      }
    }
    return false;
  }
};
//  map for storage rules in format : pair{rule, transations}
using MapRuleTransaction = std::map<Rule, Transactions, RuleComparator>;

// Class for parsing data transactions, calc and save rules wich satisfy needed support
class AssociationRules
{
public:
  AssociationRules(const double support, std::ifstream & file);
  ~AssociationRules();
  
  // Start working
  void Work();
  // parsing transactions from data file and construct map ruleTransaction.
  void Parsing();
  // Delete elements with not enough support
  void EraseWeakSupportProducts();
  // Generate rules with size - 'sizeRule'
  // getting rulles map with sufficient support and size of new rule. Return map with rules.
  MapRuleTransaction GenerateRules(MapRuleTransaction & ruleTransaction, size_t sizeRule);
  bool IsSupportSatisfy(const Transactions& data) const;
  double CalcSupport(const size_t countRuleTransactions) const;
private:
  MapRuleTransaction ruleTransaction;
  const double supportRatio;
  uint32_t countTransactions;
  // file with data 
  std::ifstream & dataFile;
  
  friend class PresenterRules;
};

