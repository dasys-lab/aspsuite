#include "Plans/TaskTwicePlan1453033761283.h"
using namespace alica;
/*PROTECTED REGION ID(eph1453033761283) ENABLED START*/ //Add additional using directives here
/*PROTECTED REGION END*/
namespace alicaAutogenerated
{
    //Plan:TaskTwicePlan

    /* generated comment
     
     Task: DefaultTask  -> EntryPoint-ID: 1453033761285

     Task: DefaultTask  -> EntryPoint-ID: 1453033782170

     */
    shared_ptr<UtilityFunction> UtilityFunction1453033761283::getUtilityFunction(Plan* plan)
    {
        /*PROTECTED REGION ID(1453033761283) ENABLED START*/

        shared_ptr < UtilityFunction > defaultFunction = make_shared < DefaultUtilityFunction > (plan);
        return defaultFunction;

        /*PROTECTED REGION END*/
    }

//State: Task1State in Plan: TaskTwicePlan

//State: Task2State in Plan: TaskTwicePlan

}
