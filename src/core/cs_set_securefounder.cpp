/* ChanServ core functions
 *
 * (C) 2003-2010 Anope Team
 * Contact us at team@anope.org
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Epona by Lara.
 * Based on the original code of Services by Andy Church.
 *
 * $Id$
 *
 */
/*************************************************************************/

#include "module.h"

class CommandCSSetSecureFounder : public Command
{
 public:
	CommandCSSetSecureFounder(const ci::string &cname, const std::string &cpermission = "") : Command(cname, 2, 2, cpermission)
	{
	}

	CommandReturn Execute(User *u, const std::vector<ci::string> &params)
	{
		ChannelInfo *ci = cs_findchan(params[0]);
		assert(ci);

		if (this->permission.empty() && ci->HasFlag(CI_SECUREFOUNDER) ? !IsFounder(u, ci) : !check_access(u, ci, CA_FOUNDER))
		{
			notice_lang(Config.s_ChanServ, u, ACCESS_DENIED);
			return MOD_CONT;
		}

		if (params[1] == "ON")
		{
			ci->SetFlag(CI_SECUREFOUNDER);
			notice_lang(Config.s_ChanServ, u, CHAN_SET_SECUREFOUNDER_ON, ci->name.c_str());
		}
		else if (params[1] == "OFF")
		{
			ci->UnsetFlag(CI_SECUREFOUNDER);
			notice_lang(Config.s_ChanServ, u, CHAN_SET_SECUREFOUNDER_OFF, ci->name.c_str());
		}
		else
			this->OnSyntaxError(u, "SECUREFOUNDER");

		return MOD_CONT;
	}

	bool OnHelp(User *u, const ci::string &)
	{
		notice_help(Config.s_ChanServ, u, CHAN_HELP_SET_SECUREFOUNDER, "SET");
		return true;
	}

	void OnSyntaxError(User *u, const ci::string &)
	{
		syntax_error(Config.s_ChanServ, u, "SET SECUREFOUNDER", CHAN_SET_SECUREFOUNDER_SYNTAX);
	}

	void OnServHelp(User *u)
	{
		notice_lang(Config.s_ChanServ, u, CHAN_HELP_CMD_SET_SECUREFOUNDER);
	}
};

class CommandCSSASetSecureFounder : public CommandCSSetSecureFounder
{
 public:
	CommandCSSASetSecureFounder(const ci::string &cname) : CommandCSSetSecureFounder(cname, "chanserv/saset/securefounder")
	{
	}

	bool OnHelp(User *u, const ci::string &)
	{
		notice_help(Config.s_ChanServ, u, CHAN_HELP_SET_SECUREFOUNDER, "SASET");
		return true;
	}

	void OnSyntaxError(User *u, const ci::string &)
	{
		syntax_error(Config.s_ChanServ, u, "SASET SECUREFOUNDER", CHAN_SASET_SECUREFOUNDER_SYNTAX);
	}
};

class CSSetSecureFounder : public Module
{
 public:
	CSSetSecureFounder(const std::string &modname, const std::string &creator) : Module(modname, creator)
	{
		this->SetAuthor("Anope");
		this->SetVersion("$Id$");
		this->SetType(CORE);

		Command *c = FindCommand(ChanServ, "SET");
		if (c)
			c->AddSubcommand(new CommandCSSetSecureFounder("SECUREFOUNDER"));

		c = FindCommand(ChanServ, "SASET");
		if (c)
			c->AddSubcommand(new CommandCSSASetSecureFounder("SECUREFOUNDER"));
	}

	~CSSetSecureFounder()
	{
		Command *c = FindCommand(ChanServ, "SET");
		if (c)
			c->DelSubcommand("SECUREFOUNDER");

		c = FindCommand(ChanServ, "SASET");
		if (c)
			c->DelSubcommand("SECUREFOUNDER");
	}
};

MODULE_INIT(CSSetSecureFounder)