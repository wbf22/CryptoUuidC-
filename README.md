# CryptoUuidStd
A cryptographically secure random UUID generator. Works on windows, linux, and mac.

This generator uses the UUID 4 random uuid standard. 

The Random.hpp file is all you need to use this generator. It uses a cryptographically secure random number
generator sourcing from '/dev/urandom' on linux/mac, and using the wincrypt library on windows. 

You can use it like so:

```c++
string uuid = Random::uuid();
```

Free to use for whatever you like, commerical or otherwise under the UniLicense (do anything you want except sue me license)
