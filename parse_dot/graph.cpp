/*
 * graph.cpp
 *
 *  Created on: Feb 3, 2014
 *      Author: nbingham
 */

#include "graph.h"
#include <parse/default/instance.h>
#include <parse/default/symbol.h>

namespace parse_dot
{

graph::graph()
{
	debug_name = "graph";
	strict = false;
}

graph::graph(tokenizer &tokens, void *data)
{
	debug_name = "graph";
	strict = false;
	parse(tokens, data);
}

graph::~graph()
{
}

void graph::parse(tokenizer &tokens, void *data)
{
	tokens.syntax_start(this);

	tokens.increment(true);
	tokens.expect("}");

	tokens.increment(false);
	tokens.expect<statement>();

	tokens.increment(true);
	tokens.expect("{");

	tokens.increment(false);
	tokens.expect<parse::instance>();

	tokens.increment(true);
	tokens.expect("graph");
	tokens.expect("digraph");
	tokens.expect("subgraph");

	tokens.increment(false);
	tokens.expect("strict");

	if (tokens.decrement(__FILE__, __LINE__, data))
	{
		tokens.next();
		strict = true;
	}

	if (tokens.decrement(__FILE__, __LINE__, data))
		type = tokens.next();

	if (tokens.decrement(__FILE__, __LINE__, data))
		id = tokens.next();

	if (tokens.decrement(__FILE__, __LINE__, data))
		tokens.next();

	while (tokens.decrement(__FILE__, __LINE__, data))
	{
		statements.push_back(statement(tokens, data));

		tokens.increment(false);
		tokens.expect<statement>();

		tokens.increment(false);
		tokens.expect(";");

		if (tokens.decrement(__FILE__, __LINE__, data))
			tokens.next();
	}

	if (tokens.decrement(__FILE__, __LINE__, data))
		tokens.next();

	tokens.syntax_end(this);
}

bool graph::is_next(tokenizer &tokens, int i, void *data)
{
	return (tokens.is_next("strict") || tokens.is_next("graph") || tokens.is_next("digraph") || tokens.is_next("subgraph"));
}

void graph::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<graph>())
	{
		tokens.register_syntax<graph>();
		statement::register_syntax(tokens);
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::symbol>();
	}
}

string graph::to_string(string tab) const
{
	string result = "";
	if (strict)
		result += "strict ";

	result += type + " " + id + "\n" + tab + "{" + "\n";
	for (int i = 0; i < (int)statements.size(); i++)
		result += tab + "\t" + statements[i].to_string(tab + "\t") + "\n";
	result += tab + "}\n";
	return result;
}

}
