/*
 * ASPGenerator.cpp
 *
 *  Created on: Jan 11, 2016
 *      Author: Stephan Opfer
 */

#include "alica_asp_solver/ASPGenerator.h"

namespace alica
{
	namespace reasoner
	{
		ASPGenerator::ASPGenerator(const void* wildcard_pointer, string wildcard_string)
			: wildcard_pointer(wildcard_pointer), wildcard_string(wildcard_string)
		{}

		// UNARY Predicates

		string ASPGenerator::plan(Plan* p, bool dotTerminated)
		{
			return "plan(" + get(p) + (dotTerminated ? ")." : ")");
		}

		string ASPGenerator::entryPoint(EntryPoint* ep, bool dotTerminated)
		{
			return "entryPoint(" + get(ep) + (dotTerminated ? ")." : ")");
		}

		string ASPGenerator::state(State* s, bool dotTerminated)
		{
			return "state(" + get(s) + (dotTerminated ? ")." : ")");
		}

		string ASPGenerator::task(Task* t, bool dotTerminated)
		{
			return "task(" + get(t) + (dotTerminated ? ")." : ")");
		}

		string ASPGenerator::failureState(State* s, bool dotTerminated)
		{
			return "failureState(" + get(s) + (dotTerminated ? ")." : ")");
		}

		string ASPGenerator::successState(State* s, bool dotTerminated)
		{
			return "successState(" + get(s) + (dotTerminated ? ")." : ")");
		}

		string ASPGenerator::planType(PlanType* pt, bool dotTerminated)
		{
			return "planType(" + get(pt) + (dotTerminated ? ")." : ")");
		}

		string ASPGenerator::successRequired(EntryPoint* ep, bool dotTerminated)
		{
			return "successRequired(" + get(ep) + (dotTerminated ? ")." : ")");
		}

		string ASPGenerator::transition(Transition* t, bool dotTerminated)
		{
			return "transition(" + get(t) + (dotTerminated ? ")." : ")");
		}

		string ASPGenerator::synchronisation(SyncTransition* sync, bool dotTerminated)
		{
			return "synchronisation(" + get(sync) + (dotTerminated ? ")." : ")");
		}


		string ASPGenerator::brokenPlan(Plan* p, bool dotTerminated)
		{
			return "brokenPlan(" + get(p) + (dotTerminated ? ")." : ")");
		}

		string ASPGenerator::cyclicPlan(Plan* p, bool dotTerminated)
		{
			return "cyclicPlan(" + get(p) + (dotTerminated ? ")." : ")");
		}

		string ASPGenerator::brokenState(State* s, bool dotTerminated)
		{
			return "brokenState(" + get(s) + (dotTerminated ? ")." : ")");
		}

		string ASPGenerator::brokenEntryPoint(EntryPoint* ep, bool dotTerminated)
		{
			return "brokenEntryPoint(" + get(ep) + (dotTerminated ? ")." : ")");
		}

		string ASPGenerator::brokenSynchronisation(SyncTransition* sync, bool dotTerminated)
		{
			return "brokenSynchronisation(" + get(sync) + (dotTerminated ? ")." : ")");
		}

		string ASPGenerator::neglocal(Condition* cond, bool dotTerminated)
		{
			return "nonlocal(" + get(cond) + (dotTerminated ? ")." : ")");
		}


		// BINARY Predicates

		string ASPGenerator::hasTask(Plan* p, Task* t, bool dotTerminated)
		{
			return "hasTask(" + get(p) + ", " + get(t) + (dotTerminated ? ")." : ")");
		}

		string ASPGenerator::hasMinCardinality(EntryPoint* ep, int minCard, bool dotTerminated)
		{
			return "hasMinCardinality(" + get(ep) + ", " + std::to_string(minCard) + (dotTerminated ? ")." : ")");
		}

		string ASPGenerator::hasMaxCardinality(EntryPoint* ep, int maxCard, bool dotTerminated)
		{
			return "hasMaxCardinality(" + get(ep) + ", " + std::to_string(maxCard) + (dotTerminated ? ")." : ")");
		}

		string ASPGenerator::hasState(Plan* p, State* s, bool dotTerminated)
		{
			return "hasState(" + get(p) + ", " + get(s) + (dotTerminated ? ")." : ")");
		}

		string ASPGenerator::hasInitialState(EntryPoint* ep, State* s, bool dotTerminated)
		{
			return "hasInitialState(" + get(ep) + ", " + get(s) + (dotTerminated ? ")." : ")");
		}

		string ASPGenerator::hasPlan(State* s, Plan* p, bool dotTerminated)
		{
			return "hasPlan(" + get(s) + ", " + get(p) + (dotTerminated ? ")." : ")");
		}

		string ASPGenerator::hasPlanType(State* s, PlanType* pt, bool dotTerminated)
		{
			return "hasPlanType(" + get(s) + ", " + get(pt) + (dotTerminated ? ")." : ")");
		}

		string ASPGenerator::hasRealisation(PlanType* pt, Plan* p, bool dotTerminated)
		{
			return "hasRealisation(" + get(pt) + ", " + get(p) + (dotTerminated ? ")." : ")");
		}

		string ASPGenerator::hasInTransition(State* s, Transition* t, bool dotTerminated)
		{
			return "hasInTransition(" + get(s) + ", " + get(t) + (dotTerminated ? ")." : ")");
		}

		string ASPGenerator::hasOutTransition(State* s, Transition* t, bool dotTerminated)
		{
			return "hasOutTransition(" + get(s) + ", " + get(t) + (dotTerminated ? ")." : ")");
		}

		string ASPGenerator::hasSynchedTransition(SyncTransition* sync, Transition* t, bool dotTerminated)
		{
			return "hasSynchedTransition(" + get(sync) + ", " + get(t) + (dotTerminated ? ")." : ")");
		}

		string ASPGenerator::brokenPlanTaskPair(Plan* p, Task* t, bool dotTerminated)
		{
			return "brokenPlanTaskPair(" + get(p) + ", " + get(t) + (dotTerminated ? ")." : ")");
		}


		// TERNARY Predicates

		string ASPGenerator::hasEntryPoint(Plan* p, Task* t, EntryPoint* ep, bool dotTerminated)
		{
			return "hasEntryPoint(" + get(p) + ", " + get(t) + ", " + get(ep) + (dotTerminated ? ")." : ")");
		}


		// RULES

		string ASPGenerator::conditionHolds(Condition* cond)
		{
			return "holds(" + get(cond) + ") :- " + cond->getConditionString();
		}


		// LITERALS

		string ASPGenerator::get(Plan* p)
		{
			if (p == this->wildcard_pointer)
				return this->wildcard_string;

			auto&& iter = this->elements.find(p->getId());
			if (iter == this->elements.end())
			{
				return this->elements[p->getId()] = ("p" + std::to_string(p->getId()));
			}
			return iter->second;
		}

		string ASPGenerator::get(EntryPoint* ep)
		{
			if (ep == this->wildcard_pointer)
				return this->wildcard_string;

			auto&& iter = this->elements.find(ep->getId());
			if (iter == this->elements.end())
			{
				return this->elements[ep->getId()] = "ep" + std::to_string(ep->getId());
			}
			return iter->second;
		}

		string ASPGenerator::get(State* s)
		{
			if (s == this->wildcard_pointer)
				return this->wildcard_string;

			auto&& iter = this->elements.find(s->getId());
			if (iter == this->elements.end())
			{
				return this->elements[s->getId()] = "s" + std::to_string(s->getId());
			}
			return iter->second;
		}

		string ASPGenerator::get(Task* t)
		{
			if (t == this->wildcard_pointer)
				return this->wildcard_string;

			auto&& iter = this->elements.find(t->getId());
			if (iter == this->elements.end())
			{
				return this->elements[t->getId()] = "tsk" + std::to_string(t->getId());
			}
			return iter->second;
		}

		string ASPGenerator::get(PlanType* pt)
		{
			if (pt == this->wildcard_pointer)
				return this->wildcard_string;

			auto&& iter = this->elements.find(pt->getId());
			if (iter == this->elements.end())
			{
				return this->elements[pt->getId()] = "pt" + std::to_string(pt->getId());
			}
			return iter->second;
		}

		string ASPGenerator::get(Transition* t)
		{
			if (t == this->wildcard_pointer)
				return this->wildcard_string;
			auto&& iter = this->elements.find(t->getId());
			if (iter == this->elements.end())
			{
				return this->elements[t->getId()] = "t" + std::to_string(t->getId());
			}
			return iter->second;
		}

		string ASPGenerator::get(SyncTransition* sync)
		{
			if (sync == this->wildcard_pointer)
				return this->wildcard_string;

			auto&& iter = this->elements.find(sync->getId());
			if (iter == this->elements.end())
			{
				return this->elements[sync->getId()] = "sync" + std::to_string(sync->getId());
			}
			return iter->second;
		}

		string ASPGenerator::get(Condition* cond)
		{
			if (sync == this->wildcard_pointer)
				return this->wildcard_string;

			auto&& iter = this->elements.find(cond->getId());
			if (iter == this->elements.end())
			{
				return this->elements[cond->getId()] = "cond" + std::to_string(cond->getId());
			}
			return iter->second;
		}
	}
}


