/* NickServ core functions
 *
 * (C) 2003-2010 Anope Team
 * Contact us at team@anope.org
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Epona by Lara.
 * Based on the original code of Services by Andy Church.
 */

/*************************************************************************/

#include "module.h"

class CommandNSSetAutoOp : public Command
{
 public:
	CommandNSSetAutoOp(const Anope::string &spermission = "") : Command("AUTOOP", 1, 2, spermission)
	{
	}

	CommandReturn Execute(User *u, const std::vector<Anope::string> &params)
	{
		NickAlias *na = findnick(params[0]);
		if (!na)
			throw CoreException("NULL na in CommandNSSetAutoOp");
		NickCore *nc = na->nc;
		
		Anope::string param = params.size() > 1 ? params[1] : "";

		if (param.equals_ci("ON"))
		{
			nc->SetFlag(NI_AUTOOP);
			u->SendMessage(NickServ, NICK_SASET_AUTOOP_ON, nc->display.c_str());
		}
		else if (param.equals_ci("OFF"))
		{
			nc->UnsetFlag(NI_AUTOOP);
			u->SendMessage(NickServ, NICK_SASET_AUTOOP_OFF, nc->display.c_str());
		}
		else
			this->OnSyntaxError(u, "AUTOOP");

		return MOD_CONT;
	}

	bool OnHelp(User *u, const Anope::string &)
	{
		u->SendMessage(NickServ, NICK_HELP_SET_AUTOOP);
		return true;
	}

	void OnSyntaxError(User *u, const Anope::string &)
	{
		SyntaxError(NickServ, u, "SET AUTOOP", NICK_SET_AUTOOP_SYNTAX);
	}

	void OnServHelp(User *u)
	{
		u->SendMessage(NickServ, NICK_HELP_CMD_SET_AUTOOP);
	}
};

class CommandNSSASetAutoOp : public CommandNSSetAutoOp
{
 public:
	CommandNSSASetAutoOp() : CommandNSSetAutoOp("nickserv/saset/autoop")
	{
	}

	bool Help(User *u, const Anope::string &)
	{
		u->SendMessage(NickServ, NICK_HELP_SASET_AUTOOP);
		return true;
	}

	void OnSyntaxError(User *u, const Anope::string &)
	{
		SyntaxError(NickServ, u, "SET AUTOOP", NICK_SASET_AUTOOP_SYNTAX);
	}

	void OnServHelp(User *u)
	{
		u->SendMessage(NickServ, NICK_HELP_CMD_SASET_AUTOOP);
	}
};

class NSSetAutoOp : public Module
{
	CommandNSSetAutoOp commandnssetautoop;
	CommandNSSASetAutoOp commandnssasetautoop;

 public:
	NSSetAutoOp(const Anope::string &modname, const Anope::string &creator) : Module(modname, creator)
	{
		this->SetAuthor("Anope");
		this->SetType(CORE);

		Command *c = FindCommand(NickServ, "SET");
		if (c)
			c->AddSubcommand(&commandnssetautoop);

		c = FindCommand(NickServ, "SASET");
		if (c)
			c->AddSubcommand(&commandnssasetautoop);
	}

	~NSSetAutoOp()
	{
		Command *c = FindCommand(NickServ, "SET");
		if (c)
			c->DelSubcommand(&commandnssetautoop);

		c = FindCommand(NickServ, "SASET");
		if (c)
			c->DelSubcommand(&commandnssasetautoop);
	}
};

MODULE_INIT(NSSetAutoOp)