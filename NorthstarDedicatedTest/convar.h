#pragma once
// taken directly from iconvar.h

// The default, no flags at all
#define FCVAR_NONE				0 

// Command to ConVars and ConCommands
// ConVar Systems
#define FCVAR_UNREGISTERED		(1<<0)	// If this is set, don't add to linked list, etc.
#define FCVAR_DEVELOPMENTONLY	(1<<1)	// Hidden in released products. Flag is removed automatically if ALLOW_DEVELOPMENT_CVARS is defined.
#define FCVAR_GAMEDLL			(1<<2)	// defined by the game DLL
#define FCVAR_CLIENTDLL			(1<<3)  // defined by the client DLL
#define FCVAR_HIDDEN			(1<<4)	// Hidden. Doesn't appear in find or auto complete. Like DEVELOPMENTONLY, but can't be compiled out.

// ConVar only
#define FCVAR_PROTECTED			(1<<5)  // It's a server cvar, but we don't send the data since it's a password, etc.  Sends 1 if it's not bland/zero, 0 otherwise as value
#define FCVAR_SPONLY			(1<<6)  // This cvar cannot be changed by clients connected to a multiplayer server.
#define	FCVAR_ARCHIVE			(1<<7)	// set to cause it to be saved to vars.rc
#define	FCVAR_NOTIFY			(1<<8)	// notifies players when changed
#define	FCVAR_USERINFO			(1<<9)	// changes the client's info string

#define FCVAR_PRINTABLEONLY		(1<<10)  // This cvar's string cannot contain unprintable characters ( e.g., used for player name etc ).

#define FCVAR_GAMEDLL_FOR_REMOTE_CLIENTS		(1<<10)  // When on concommands this allows remote clients to execute this cmd on the server. 
														 // We are changing the default behavior of concommands to disallow execution by remote clients without
														 // this flag due to the number existing concommands that can lag or crash the server when clients abuse them.

#define FCVAR_UNLOGGED			(1<<11)  // If this is a FCVAR_SERVER, don't log changes to the log file / console if we are creating a log
#define FCVAR_NEVER_AS_STRING	(1<<12)  // never try to print that cvar

// It's a ConVar that's shared between the client and the server.
// At signon, the values of all such ConVars are sent from the server to the client (skipped for local
//  client, of course )
// If a change is requested it must come from the console (i.e., no remote client changes)
// If a value is changed while a server is active, it's replicated to all connected clients
#define FCVAR_REPLICATED		(1<<13)	// server setting enforced on clients, TODO rename to FCAR_SERVER at some time
#define FCVAR_CHEAT				(1<<14) // Only useable in singleplayer / debug / multiplayer & sv_cheats
#define FCVAR_SS				(1<<15) // causes varnameN where N == 2 through max splitscreen slots for mod to be autogenerated
#define FCVAR_DEMO				(1<<16) // record this cvar when starting a demo file
#define FCVAR_DONTRECORD		(1<<17) // don't record these command in demofiles
#define FCVAR_SS_ADDED			(1<<18) // This is one of the "added" FCVAR_SS variables for the splitscreen players
#define FCVAR_RELEASE			(1<<19) // Cvars tagged with this are the only cvars avaliable to customers
#define FCVAR_RELOAD_MATERIALS	(1<<20)	// If this cvar changes, it forces a material reload
#define FCVAR_RELOAD_TEXTURES	(1<<21)	// If this cvar changes, if forces a texture reload

#define FCVAR_NOT_CONNECTED		(1<<22)	// cvar cannot be changed by a client that is connected to a server
#define FCVAR_MATERIAL_SYSTEM_THREAD (1<<23)	// Indicates this cvar is read from the material system thread
#define FCVAR_ARCHIVE_GAMECONSOLE	(1<<24) // cvar written to config.cfg on the Xbox

#define FCVAR_SERVER_CAN_EXECUTE	(1<<28)// the server is allowed to execute this command on clients via ClientCommand/NET_StringCmd/CBaseClientState::ProcessStringCmd.
#define FCVAR_SERVER_CANNOT_QUERY	(1<<29)// If this is set, then the server is not allowed to query this cvar's value (via IServerPluginHelpers::StartQueryCvarValue).
#define FCVAR_CLIENTCMD_CAN_EXECUTE	(1<<30)	// IVEngineClient::ClientCmd is allowed to execute this command. 
											// Note: IVEngineClient::ClientCmd_Unrestricted can run any client command.

#define FCVAR_ACCESSIBLE_FROM_THREADS	(1<<25)	// used as a debugging tool necessary to check material system thread convars
// #define FCVAR_AVAILABLE			(1<<26)
// #define FCVAR_AVAILABLE			(1<<27)
// #define FCVAR_AVAILABLE			(1<<31)

class ConCommand;

// still need to map out functions and that for this, would be nice to be able to get actual values out of these in native
// also i sure do hope this size is right because there's a fairly decent chance it isn't
class ConVar
{
private:
	// if there are ever crashes caused by modifying custom cvars, check this
	unsigned char unknown[0x88];
public:
    virtual	void    			EngineDestructor(void) {}
    virtual	bool				IsCommand(void) const { return false; }
    virtual bool				IsFlagSet(int flag) { return false; }
    virtual void				AddFlags(int flags) {}
    virtual void				RemoveFlags(int flags) {}
    virtual int					GetFlags() const { return 0; }
    virtual const char*         GetName(void) const { return nullptr; }
    virtual const char*         GetHelpText(void) const { return nullptr; }
    virtual bool				IsRegistered(void) const { return false; }
};

class ICvar
{
public:
    struct VTable
    {
        void* unknown[10];
        void(*UnregisterConCommand) (ICvar* cvar, ConCommand* pCommandBase);
        void* unknown2[28];
        ConVar*(*FindVar)(const char* var_name);
    };

    VTable* m_vtable;
};


ConVar* RegisterConVar(const char* name, const char* defaultValue, int flags, const char* helpString);
void InitialiseConVars(HMODULE baseAddress);