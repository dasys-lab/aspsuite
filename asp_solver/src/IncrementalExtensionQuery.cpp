#include "reasoner/asp/IncrementalExtensionQuery.h"
#include "reasoner/asp/Solver.h"
namespace reasoner
{
namespace asp
{

std::vector<IncrementalExtensionQuery*> IncrementalExtensionQuery::queries = std::vector<IncrementalExtensionQuery*>();
int IncrementalExtensionQuery::queryId = 0;

void IncrementalExtensionQuery::clear()
{
    //    for(auto query : IncrementalExtensionQuery::queries) { //FIXME double free - who is deleting these?
    //        delete query;
    //    }

    IncrementalExtensionQuery::queries.clear();
    IncrementalExtensionQuery::queryId = 0;
}

//#define QUERY_DEBUG
void IncrementalExtensionQuery::cleanUp()
{
    // Hack - first is base term query
    for (int i = 1; i < IncrementalExtensionQuery::queries.size(); ++i) {
        auto query = IncrementalExtensionQuery::queries.at(i);
        query->getSolver()->assignExternal(*(query->external), Clingo::TruthValue::False);
    }
}

bool IncrementalExtensionQuery::isPresent(int horizon)
{
    // FIXME first is base term query
    return IncrementalExtensionQuery::queries.size() > horizon;
}

void IncrementalExtensionQuery::activate(int horizon)
{

    auto query = IncrementalExtensionQuery::queries.at(horizon);
    query->getSolver()->assignExternal(*(query->external), Clingo::TruthValue::True);
}
IncrementalExtensionQuery::IncrementalExtensionQuery(int queryID, Solver* solver, Term* term)
        : ExtensionQuery(queryID, solver, term)
{
    IncrementalExtensionQuery::queries.push_back(this);
    this->type = QueryType::IncrementalExtension;

    std::stringstream ss;
    ss << "incquery"; // << IncrementalExtensionQuery::queryId;
    this->queryProgramSection = ss.str();
    this->lastQuerySection = "";

    Clingo::SymbolVector paramsVec;
    for (auto param : this->term->getProgramSectionParameters()) {
        paramsVec.push_back(this->solver->parseValue(param.second));
        // TODO re-work, used for horizon externals

        this->lastQuerySection = this->queryProgramSection + std::to_string(stoi(param.second) - 1);
        this->queryProgramSection = this->queryProgramSection + param.second;
    }
    if (term->getProgramSectionParameters().empty()) {
        this->queryProgramSection = this->queryProgramSection + "0";
    }

    this->generateQueryProgram();
    this->solver->add(this->queryProgramSection.c_str(), {}, this->queryProgram.c_str());
    this->solver->ground({{this->queryProgramSection.c_str(), paramsVec}}, nullptr);
    this->external = std::make_shared<Clingo::Symbol>(this->solver->parseValue(this->externalName));
    this->solver->assignExternal(*(this->external), Clingo::TruthValue::True);
}

void IncrementalExtensionQuery::onModel(Clingo::Model& clingoModel)
{
    // incremental query was successful, so the next inc query can be queried
    IncrementalExtensionQuery::queryId += 1;
    // IncrementalExtensionQuery::cleanUp();
}

void IncrementalExtensionQuery::generateQueryProgram()
{
    std::stringstream ss;
    if (!term->getProgramSectionParameters().empty() && !term->getRules().empty()) {
        ss << "(";
        for (int i = 0; i < term->getProgramSectionParameters().size(); ++i) {
            ss << term->getProgramSectionParameters().at(i).first;
            if (i != term->getProgramSectionParameters().size() - 1) {
                ss << ", ";
            }
        }
        ss << ")";
    }

    auto wrappedParams = ss.str();
    auto programSection = this->queryProgramSection + wrappedParams;
    this->externalName = "external" + this->queryProgramSection;

    std::stringstream queryProgramStream;
    queryProgramStream << "#program " << this->queryProgramSection + ss.str() << ".\n";
    queryProgramStream << "#external " << externalName << ".\n";

    // 1. collect head predicates for encapsulating these for module property
    for (auto& rule : term->getRules()) {
        rememberPredicates(determineHeadPredicates(rule));
    }
    for (auto& fact : term->getFacts()) {
        rememberPredicates(determineHeadPredicates(fact));
    }

#ifdef ASPQUERY_DEBUG
    std::cout << "INPUT PROGRAM: " << std::endl;
    for (auto& rule : term->getRules()) {
        std::cout << rule << std::endl;
    }
    for (auto& fact : term->getFacts()) {
        std::cout << fact << std::endl;
    }
    std::cout << std::endl;

    std::cout << "HEAD VALUES: " << std::endl;
    for (auto& predicateArityPair : predicatesToAritiesMap) {
        std::cout << predicateArityPair.first << ": ";
        for (auto& arity : predicateArityPair.second) {
            std::cout << arity << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
#endif

    // 2. generate knowledge base capturing rules
    for (auto& headPredicate : predicatesToAritiesMap) {
        for (auto& arity : headPredicate.second) {
            queryProgramStream << createKBCapturingRule(headPredicate.first, arity, this->queryProgramSection);
        }
    }

    // 3. taking care of module property after head predicates where collected
    for (auto& fact : term->getFacts()) {
        queryProgramStream << expandFactModuleProperty(fact);
    }
    for (auto& rule : term->getRules()) {
        queryProgramStream << expandRuleModuleProperty(rule, (this->lastQuerySection.empty() ? this->queryProgramSection : this->lastQuerySection));
    }
    this->queryProgram = queryProgramStream.str();

#ifdef ASPQUERY_DEBUG
    std::cout << "RESULT: " << std::endl << this->queryProgram << std::endl;
#endif
}

void IncrementalExtensionQuery::removeExternal()
{
    this->solver->assignExternal(*(this->external.get()), Clingo::TruthValue::False);
}
} // namespace asp
} // namespace reasoner