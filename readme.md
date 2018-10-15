libtheme
========

this is a theming library for the plan 9 graphics system.

theme files are in ndb format. there is a sample in lib/theme.

theme files are read from $home/lib/theme, or /lib/theme if there is no theme file in $home.

applications using the theme library should call `themeget` to find color values. for example:

```
	ulong col = themeget("rio", "background", DBlack);
```

this will look up an ndb record in the theme file that looks like this:

```
	theme=rio
		background=red
```

if the key `background` doesn't exist, it will fall back to DBlack.

installation
============

```
mk install
cp lib/theme $home/lib/theme
B $home/lib/theme
```

color names
===========

some predefined color names are in the library, taken from allocimage(2):

- black
- white
- red
- green
- blue
- cyan
- magenta
- paleyellow
- darkyellow
- darkgreen
- palegreen
- medgreen
- darkblue
- palebluegreen
- paleblue
- bluegreen
- greygreen
- palegreygreen
- yellowgreen
- medblue
- greyblue
- palegreyblue
- purpleblue

patches for software
====================

there are some patches available to make existing software use libtheme.

see [libtheme-patches](https://bitbucket.org/mischief/libtheme-patches) for details.
