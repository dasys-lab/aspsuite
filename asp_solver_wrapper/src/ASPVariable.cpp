/*
 * Variable.cpp
 *
 *  Created on: Jul 13, 2016
 *      Author: Stefan Jakob
 */

#include <asp_solver_wrapper/ASPVariable.h>

namespace alica
{
	namespace reasoner
	{
		int ASPVariable::var_id = 0;
		ASPVariable::ASPVariable()
		{
			ownId = var_id++;

		}

		ASPVariable::~ASPVariable()
		{
			// TODO Auto-generated destructor stub
		}

	} /* namespace reasoner */
} /* namespace alica */