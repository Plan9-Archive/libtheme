#include <u.h>
#include <libc.h>
#include <draw.h>
#include <bio.h>
#include <ndb.h>

typedef struct colmap colmap;
struct colmap {
	char *name;
	ulong value;
};

static colmap coltab[] = {
	{"black",		DBlack},
	{"white", 		DWhite},
	{"red",			DRed},
	{"green",		DGreen},
	{"blue",		DBlue},
	{"cyan",		DCyan},
	{"magenta",		DMagenta},
	{"paleyellow",		DPaleyellow},
	{"darkyellow",		DDarkyellow},
	{"darkgreen",		DDarkgreen},
	{"palegreen",		DPalegreen},
	{"medgreen",		DMedgreen},
	{"darkblue",		DDarkblue},
	{"palebluegreen",	DPalebluegreen},
	{"paleblue",		DPaleblue},
	{"bluegreen",		DBluegreen},
	{"greygreen",		DGreygreen},
	{"palegreygreen",	DPalegreygreen},
	{"yellowgreen",		DYellowgreen},
	{"medblue",		DMedblue},
	{"greyblue",		DGreyblue},
	{"palegreyblue",	DPalegreyblue},
	{"purpleblue",		DPurpleblue},
};

static ulong
name2col(char *name)
{
	int i;

	for(i = 0; i < nelem(coltab); i++){
		if(strcmp(name, coltab[i].name) == 0)
			return coltab[i].value;
	}

	return DNotacolor;
}

static Ndb *themedb;
static QLock themelock;
static char *themename;

void
themeinit(char *theme)
{
	char *home, *db;

	themename = strdup(theme);
	if(themename == nil)
		sysfatal("theme: strdup: %r");

	home = getenv("home");
	if(home == nil)
		home = "";

	db = smprint("%s/lib/theme", home);
	assert(db != nil);

	qlock(&themelock);
	assert(themedb == nil);

	themedb = ndbopen(db);
	/* if there's no db in $home, try /lib. */
	if(themedb == nil)
		themedb = ndbopen("/lib/theme");

	/* if themedb is nil, silently fail. */

	qunlock(&themelock);
	free(db);
	free(home);
}

void
themeend(void)
{
	free(themename);
	qlock(&themelock);
	if(themedb != nil)
		ndbclose(themedb);
	qunlock(&themelock);
}

static ulong
convcolor(char *color)
{
	ulong r;
	char *p;

	r = strtoul(color, &p, 0);
	/* successfully parsed? */
	if(color != p && *p == '\0')
		return r;

	return name2col(color);
}

ulong
themeget(char *color, ulong def)
{
	ulong r;
	Ndbs s;
	char *val;

	r = def;

	qlock(&themelock);

	/* if themedb is nil, return default. */
	if(themedb == nil)
		goto done;

	/* first try with theme */
	val = ndbgetvalue(themedb, &s, "theme", themename, color, nil);
	if(val != nil){
		r = convcolor(val);
		if(r == DNotacolor)
			r = def;

		goto done;
	}

	/* no theme */
	val = ndbgetvalue(themedb, &s, "theme", "default", color, nil);
	if(val != nil){
		r = convcolor(val);
		if(r == DNotacolor)
			r = def;

		goto done;
	}

done:
	qunlock(&themelock);

	return r;
}
