/*
 * Variable.cpp
 *
 *  Created on: Jul 13, 2016
 *      Author: Stefan Jakob
 */

#include "../include/asp_commons/ASPVariable.h"

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
