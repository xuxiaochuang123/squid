
/*
 * $Id: ACL.h,v 1.12 2004/08/30 05:12:30 robertc Exp $
 *
 *
 * SQUID Web Proxy Cache          http://www.squid-cache.org/
 * ----------------------------------------------------------
 *
 *  Squid is the result of efforts by numerous individuals from
 *  the Internet community; see the CONTRIBUTORS file for full
 *  details.   Many organizations have provided support for Squid's
 *  development; see the SPONSORS file for full details.  Squid is
 *  Copyrighted (C) 2001 by the Regents of the University of
 *  California; see the COPYRIGHT file for full details.  Squid
 *  incorporates software developed and/or copyrighted by other
 *  sources; see the CREDITS file for full details.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
 *
 *
 * Copyright (c) 2003, Robert Collins <robertc@squid-cache.org>
 */

#ifndef SQUID_ACL_H
#define SQUID_ACL_H
#include "Array.h"

/* acl.c */
SQUIDCEXTERN void aclDestroyAccessList(acl_access **list);
SQUIDCEXTERN void aclDestroyAcls(ACL **);
SQUIDCEXTERN void aclDestroyAclList(acl_list **);
SQUIDCEXTERN void aclParseAccessLine(acl_access **);
SQUIDCEXTERN void aclParseAclList(acl_list **);
SQUIDCEXTERN int aclIsProxyAuth(const char *name);
SQUIDCEXTERN err_type aclGetDenyInfoPage(acl_deny_info_list ** head, const char *name);

SQUIDCEXTERN void aclParseDenyInfoLine(struct _acl_deny_info_list **);

SQUIDCEXTERN void aclDestroyDenyInfoList(struct _acl_deny_info_list **);
SQUIDCEXTERN wordlist *aclDumpGeneric(const ACL *);
SQUIDCEXTERN void aclCacheMatchFlush(dlink_list * cache);
extern void dump_acl_access(StoreEntry * entry, const char *name, acl_access * head);
int aclPurgeMethodInUse(acl_access * a);
extern const char *AclMatchedName;	/* NULL */

class ACL
{

public:
    void *operator new(size_t);
    void operator delete(void *);

    static ACL *Factory (char const *);
    static void ParseAclLine(ACL ** head);
    static void Initialize();
    static ACL* FindByName(const char *name);

    ACL();
    virtual ~ACL();
    virtual ACL *clone()const = 0;
    virtual void parse() = 0;
    virtual char const *typeString() const = 0;
    virtual bool isProxyAuth() const;
    virtual bool requiresRequest() const;
    virtual bool requiresReply() const;
    virtual int match(ACLChecklist * checklist) = 0;
    virtual wordlist *dumpGeneric() const;
    virtual wordlist *dump() const = 0;
    virtual bool valid () const =0;
    int checklistMatches(ACLChecklist *);

    int cacheMatchAcl(dlink_list * cache, ACLChecklist *);
    virtual int matchForCache(ACLChecklist *checklist);

    virtual void prepareForUse() {}

    char name[ACL_NAME_SZ];
    char *cfgline;
    ACL *next;

public:

    class Prototype
    {

    public:
        Prototype ();
        Prototype (ACL const *, char const *);
        ~Prototype();
        static bool Registered(char const *);
        static ACL *Factory (char const *);

    private:
        ACL const*prototype;
        char const *typeString;

    private:
        static Vector<Prototype const *> * Registry;
        static void *Initialized;
        typedef Vector<Prototype const*>::iterator iterator;
        typedef Vector<Prototype const*>::const_iterator const_iterator;
        void registerMe();
    };
};

class acl_access
{

public:
    void *operator new(size_t);
    void operator delete(void *);
    bool containsPURGE() const;
    allow_t allow;
    acl_list *aclList;
    char *cfgline;
    acl_access *next;

private:
    CBDATA_CLASS(acl_access);
};

class ACLList
{

public:
    MEMPROXY_CLASS(ACLList);

    ACLList();
    void negated(bool isNegated);
    bool matches (ACLChecklist *)const;
    int op;
    ACL *_acl;
    ACLList *next;
};

MEMPROXY_CLASS_INLINE(ACLList)

typedef ACLList acl_list;
#endif /* SQUID_ACL_H */
