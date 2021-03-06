# SQLite4 Log-structured merge-tree (LSM) compression demo

This repository contains a demo of using the SQLite4 [LSM
interface][lsm], using [lz4][] for compression support at the database
level.

[lsm]: http://sqlite.org/src4/doc/trunk/www/lsmusr.wiki
[lz4]: https://code.google.com/p/lz4/

SQLite4 is not officially released. You should not use the source code
under `lsm/sqlite4.c` for any purpose until it is officially released
by the SQLite developers. I can't be held responsible for your
mistakes.

The copy of sqlite under `lsm/sqlite4.c` was generated on May 4th, 2013.

# Performance and size numbers

The included example program inserts every line of
`/usr/share/dict/words` into a database and closes it. On my mid-2011
Core i5, with Ubuntu 12.10, an intel SSD, and Clang 3.3svn, I get:

```
$ make
...
$ time ./lsm-test
Inserting all keys/values (using /usr/share/dict/words)
Done. Now retrieving keys...
Done.
./lsm-test  0.39s user 0.22s system 84% cpu 0.724 total
$ time ./lsm-lz4-test
Inserting all keys/values (using /usr/share/dict/words)
Done. Now retrieving keys...
Done.
./lsm-lz4-test  0.39s user 0.32s system 92% cpu 0.764 total
$ du -h test.db test-lz4.db
4.4M	test.db
2.4M	test-lz4.db
```

# Join in

Be sure to read the [contributing guidelines][contribute]. File bugs
in the GitHub [issue tracker][].

Master [git repository][gh]:

* `git clone https://github.com/thoughtpolice/sqlite4_lsm_lz4.git`

There's also a [BitBucket mirror][bb]:

* `git clone https://bitbucket.org/thoughtpolice/sqlite4_lsm_lz4.git`

# Authors

See [AUTHORS.txt](https://raw.github.com/thoughtpolice/sqlite4_lsm_lz4/master/AUTHORS.txt).

# License

BSD2, same as LZ4. See
[LICENSE.txt](https://raw.github.com/thoughtpolice/sqlite4_lsm_lz4/master/LICENSE.txt)
for terms of copyright and redistribution.

[contribute]: https://github.com/thoughtpolice/sqlite4_lsm_lz4/blob/master/CONTRIBUTING.md
[issue tracker]: http://github.com/thoughtpolice/sqlite4_lsm_lz4/issues
[gh]: http://github.com/thoughtpolice/sqlite4_lsm_lz4
[bb]: http://bitbucket.org/thoughtpolice/sqlite4_lsm_lz4
