Preloadable library to redirect syslog(3) output to stderr
======================================================================

  * Copyright (c) 2019 SATOH Fumiyasu @ OSS Technology Corp., Japan
  * License: BSD-like (2-clause)
  * URL: <https://GitHub.com/fumiyas/syslog-redirect>
  * Twitter: <https://twitter.com/satoh_fumiyasu>

What's this?
---------------------------------------------------------------------

This is a `$LD_PRELOAD`-able library and a wrapper script to
run a command with the special `syslog`(3) function for redirecting
syslog output to the standard error (stderr).

Use case
---------------------------------------------------------------------

Run a syslog-aware service in a container environment (e.g., Docker)
without a syslog daemon or the /dev/log.

How to build
---------------------------------------------------------------------

Required packages: autoconf, automake, libtool, make, cc (gcc or misc)

```console
$ sh autogen.sh
$ ./configure --prefix=/usr/local
$ make
$ sudo make install
```

Usage
---------------------------------------------------------------------

Run a command with `$LD_PRELOAD`-able library:

```console
$ LD_PRELOAD=/usr/local/lib/syslog-redirect.so syslog-aware-command
...
```

or:

```console
$ /usr/local/bin/syslog-redirect
Usage: /usr/local/bin/syslog-redirect [OPTIONS] COMMAND [ARGUMENT ...]

Options:
 -l, --log-level LEVEL
    Specify log level
    Default: 4 (LOG_WARNING)
 --identity
    Print identity string to log
 --facility
    Print facility string to log
 --level
    Print log level string to log
 --all
    Print identity, facility and log level string to log
$ /usr/local/bin/syslog-redirect --log-level 7 --all syslog-aware-command
...
```

Note
---------------------------------------------------------------------

  * If a target command does not call `openlog`(3) before `syslog`(3),
    all `$SYSLOG_REDIRECT_*` options do NOT function.
  * Some `logger`(1) implementations do NOT use `syslog`(3) function,
    thus `syslog-redirect` does not affect it.
