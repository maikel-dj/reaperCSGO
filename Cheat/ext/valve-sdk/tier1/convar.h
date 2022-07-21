#pragma once

#include "IConVar.h"
#include "../ICvar.h"
#include <winerror.h>

#ifdef _WIN32
#define FORCEINLINE_CVAR FORCEINLINE
#elif POSIX
#define FORCEINLINE_CVAR inline
#else
#error "implement me"
#endif

class IConCommandBaseAccessor
{
public:
	// Flags is a combination of FCVAR flags in cvar.h.
	// hOut is filled in with a handle to the variable.
	virtual bool RegisterConCommandBase(ConCommandBase* pVar) = 0;
};

class ConCommandBase
{
	friend class CCvar;
	friend class ConVar;
	friend class ConCommand;
	friend void ConVar_Register(int nCVarFlag, IConCommandBaseAccessor* pAccessor);
	friend void ConVar_PublishToVXConsole();

	// FIXME: Remove when ConVar changes are done
	friend class CDefaultCvar;

public:
	ConCommandBase(void);
	ConCommandBase(const char* pName, const char* pHelpString = 0,
		int flags = 0);

	virtual						~ConCommandBase(void);

	virtual	bool				IsCommand(void) const;

	// Check flag
	virtual bool				IsFlagSet(int flag) const;
	// Set flag
	virtual void				AddFlags(int flags);

	// Return name of cvar
	virtual const char* GetName(void) const;

	// Return help text for cvar
	virtual const char* GetHelpText(void) const;

	// Deal with next pointer
	const ConCommandBase* GetNext(void) const;
	ConCommandBase* GetNext(void);

	virtual bool				IsRegistered(void) const;

	// Returns the DLL identifier
	virtual CVarDLLIdentifier_t	GetDLLIdentifier() const;

protected:
	virtual void				CreateBase(const char* pName, const char* pHelpString = 0,
		int flags = 0);

	// Used internally by OneTimeInit to initialize/shutdown
	virtual void				Init();
	void						Shutdown();

	// Internal copy routine ( uses new operator from correct module )
	char* CopyString(const char* from);

private:
	// Next ConVar in chain
	// Prior to register, it points to the next convar in the DLL.
	// Once registered, though, m_pNext is reset to point to the next
	// convar in the global list
	ConCommandBase* m_pNext;

	// Has the cvar been added to the global list?
	bool						m_bRegistered;

	// Static data
	const char* m_pszName;
	const char* m_pszHelpString;

	// ConVar flags
	int							m_nFlags;

protected:
	// ConVars add themselves to this list for the executable. 
	// Then ConVar_Register runs through  all the console variables 
	// and registers them into a global list stored in vstdlib.dll
	static ConCommandBase* s_pConCommandBases;

	// ConVars in this executable use this 'global' to access values.
	static IConCommandBaseAccessor* s_pAccessor;
};


class ConVar : public ConCommandBase, public IConVar
{
	friend class CCvar;
	friend class ConVarRef;

public:
	typedef ConCommandBase BaseClass;

	ConVar(const char* pName, const char* pDefaultValue, int flags = 0);

	ConVar(const char* pName, const char* pDefaultValue, int flags,
		const char* pHelpString);
	ConVar(const char* pName, const char* pDefaultValue, int flags,
		const char* pHelpString, bool bMin, float fMin, bool bMax, float fMax);
	ConVar(const char* pName, const char* pDefaultValue, int flags,
		const char* pHelpString, FnChangeCallback_t callback);
	ConVar(const char* pName, const char* pDefaultValue, int flags,
		const char* pHelpString, bool bMin, float fMin, bool bMax, float fMax,
		FnChangeCallback_t callback);

	virtual						~ConVar(void);

	virtual bool				IsFlagSet(int flag) const;
	virtual const char* GetHelpText(void) const;
	virtual bool				IsRegistered(void) const;
	virtual const char* GetName(void) const;
	virtual void				AddFlags(int flags);
	virtual	bool				IsCommand(void) const;

	// Install a change callback (there shouldn't already be one....)
	void InstallChangeCallback(FnChangeCallback_t callback);

	// Retrieve value
	FORCEINLINE_CVAR float			GetFloat(void) const;
	FORCEINLINE_CVAR int			GetInt(void) const;
	FORCEINLINE_CVAR bool			GetBool() const { return !!GetInt(); }
	FORCEINLINE_CVAR char const* GetString(void) const;

	// Any function that allocates/frees memory needs to be virtual or else you'll have crashes
	//  from alloc/free across dll/exe boundaries.

	// These just call into the IConCommandBaseAccessor to check flags and set the var (which ends up calling InternalSetValue).
	virtual void				SetValue(const char* value);
	virtual void				SetValue(float value);
	virtual void				SetValue(int value);

	// Reset to default value
	void						Revert(void);

	// True if it has a min/max setting
	bool						GetMin(float& minVal) const;
	bool						GetMax(float& maxVal) const;
	const char* GetDefault(void) const;
	void						SetDefault(const char* pszDefault);

private:
	// Called by CCvar when the value of a var is changing.
	virtual void				InternalSetValue(const char* value);
	// For CVARs marked FCVAR_NEVER_AS_STRING
	virtual void				InternalSetFloatValue(float fNewValue);
	virtual void				InternalSetIntValue(int nValue);

	virtual bool				ClampValue(float& value);
	virtual void				ChangeStringValue(const char* tempVal, float flOldValue);

	virtual void				Create(const char* pName, const char* pDefaultValue, int flags = 0,
		const char* pHelpString = 0, bool bMin = false, float fMin = 0.0,
		bool bMax = false, float fMax = false, FnChangeCallback_t callback = 0);

	// Used internally by OneTimeInit to initialize.
	virtual void				Init();
	int GetFlags() { return m_pParent->m_nFlags; }
private:

	// This either points to "this" or it points to the original declaration of a ConVar.
	// This allows ConVars to exist in separate modules, and they all use the first one to be declared.
	// m_pParent->m_pParent must equal m_pParent (ie: m_pParent must be the root, or original, ConVar).
	ConVar* m_pParent;

	// Static data
	const char* m_pszDefaultValue;

	// Value
	// Dynamically allocated
	char* m_pszString;
	int							m_StringLength;

	// Values
	float						m_fValue;
	int							m_nValue;

	// Min/Max values
	bool						m_bHasMin;
	float						m_fMinVal;
	bool						m_bHasMax;
	float						m_fMaxVal;

	// Call this function when ConVar changes
	FnChangeCallback_t			m_fnChangeCallback;
};