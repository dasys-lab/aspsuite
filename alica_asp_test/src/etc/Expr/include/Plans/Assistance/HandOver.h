#ifndef HandOver_H_
#define HandOver_H_

#include "DomainBehaviour.h"
/*PROTECTED REGION ID(inc1463404665666) ENABLED START*/ //Add additional includes here
/*PROTECTED REGION END*/
namespace alica
{
    class HandOver : public DomainBehaviour
    {
    public:
        HandOver();
        virtual ~HandOver();
        virtual void run(void* msg);
        /*PROTECTED REGION ID(pub1463404665666) ENABLED START*/ //Add additional public methods here
        /*PROTECTED REGION END*/
    protected:
        virtual void initialiseParameters();
        /*PROTECTED REGION ID(pro1463404665666) ENABLED START*/ //Add additional protected methods here
        /*PROTECTED REGION END*/
    private:
        /*PROTECTED REGION ID(prv1463404665666) ENABLED START*/ //Add additional private methods here
        /*PROTECTED REGION END*/};
} /* namespace alica */

#endif /* HandOver_H_ */
