#ifndef CONDITIONCREATOR_H_
#define CONDITIONCREATOR_H_

using namespace std;

#include "engine/IConditionCreator.h"
#include <memory>
#include "iostream"

namespace alica
{
    class BasicCondition;

    class ConditionCreator : public alica::IConditionCreator
    {
    public:
        ConditionCreator();
        virtual ~ConditionCreator();
        shared_ptr<BasicCondition> createConditions(long conditionConfId);
    };

} /* namespace alica */

#endif /* ALICA_ALICA_TEST_SRC_TESTCONSTRAINTCREATOR_H_ */
