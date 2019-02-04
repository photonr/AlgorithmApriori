#pragma once
#include "AssociationRules.h"

enum class TypeSorting {IncreaseSupport, DecreaseSupport, None};
// class for present received rules in some format
class PresenterRules
{
public:
  PresenterRules();
  ~PresenterRules();

  static std::ostringstream GetRules(const AssociationRules& parsing, const TypeSorting typeSort = TypeSorting::None, const size_t countRules = 100);
  static void SortRules(std::vector<std::pair<std::vector<uint32_t>, double>>& rules, const TypeSorting typeSort);

private:
};