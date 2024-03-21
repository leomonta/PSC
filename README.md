# Personal Server Chat (PSC)

A simple client-server encrypted chat built on top TCP for personal use and small groups

---

## The Communication is based on a binary custom protocol described here

### General requirements

#### Who is this?

-   Who is doing the action (user or server)
    -   UUID 32bits of random bits given by the server to the client. Representable in 8 hex digits
        -   The servers check that no one in the server has the same UUID. obv
        -   32bits allow for 2^32 4 bln UUID, more than enough
    -   The server is UUID 00...00, unmistakable
        -   Might give the server an actual UUID, but the actual identification also requires the key so it should be fine

#### When did it happen?

-   Time of the message being sent
    -   Unix timestamp
        -   Formatted in chat as GMT timestamp formatted following the RFC 1123
            -   An example -> Sun, 06 Nov 1994 08:49:37, 25 bytes long, the GMT is implied
    -   The server should echo back to the client the time at which it has received the message

#### What do you want?

-   What action is being performed (Post a message to the server, get a message from the server)
    -   Get a resource
    -   Post a resource
    -   Patch (edit) a past resource
        -   The specific resource ID is specified in the body
    -   Each with a variant
        -   Text only, lightweight.
        -   Bin only, if you also need the binary resource

#### The actual body

-   The Body of the message
    -   The text plus some referencies to media and formatting
    -   The binary data is referenced by the message should be sent with a dedicated message, preferrably before the actual message

#### Version

-   Protocol Version
    -   4bits.4bits, only major and minor from 0.0 to 16.16
        -   1.0 for now

#### How much is being transferred

-   Content lenght in Bytes excluding the header
    -   Use remaining size in the header, 21 bits, 2097151B, max 2MiB data

### Digital signature (WIP)

### Bit map

```
    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F
00 |     VM    |     Vm    | MM  | V|                            length                            |
20 |                                             UUID                                              |
40 |                                           timestamp                                           |
60 |                                             Body                                              |
...|                                                                                               |

VM        => Version Major

Vm        => Version Minor

M M       => 00 GET
             01 POST
             10 PATCH

V         => 0 TEXT
             1 BIN

length    => Lenght of the body in size

UUID      => Unique User ID

timestamp => 32bit unigned int unix timestamp

Body      => text or binary data

```

### Endiannes

All Values should be stored in big endian

---

## Network

### Header

The general header is 64bytes long and contains the minimum data I can think of that is always needed

This header does not contain sensitive data, the UUID means who is talking for the server only, so it's not needed to encrypt it since the identification is based on keys and signatures

### Port

61970

### Protocol base

Tcp

---

## Storage

The data that needs to be stored in quite simple

We need to store users, their messages and their resources it possible to just put these in a file

```
bin
├── PSCS
├── users.dat
├── msgs
│   ├── 0-100
│   ├── 101-200
│   └── ...-...
└── res
    ├── a0b1c2d3.png
    ├── a1b2c3d4.txt
    └── f7d539bc.dae
```

### Users

Need UUID, username and public key

Even if there were 1000 users it wouldn't be a problem to just search the correct one, it's not that much data, thus no need for a complex data structure

```
    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F
 00 |                                             UUID                                              |
 20 |                                             uname                                             |
 64 more times
420 |                                             Key                                               |
 ...|...............................................................................................|

UUID => The User Unique IDentificator of the user
Body => 256 bytes for an UTF-8 user name to show to other users
Key  => The user public key, used to confirm its digital signature
```

### Messages

```
    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F
00 |                                             UUID                                              |
20 |                                             MSGID                                             |
40 |                                        Send Timestamp                                         |
60 |                                        Recv Timestamp                                         |
80 |                                             Size                                              |
A0 |                                             Body                                              |
...|...............................................................................................|
...|                                             Resname                                           |
...|...............................................................................................|

UUID    => The author UUID
MSGID   => The message ID, an unsigned 32bit integer progressing from 0
Body    => The body of the message as a UTF-8 list
Resname => A list of resources names, names are always 64bits
```

### Resources

Resources are genrally binary files associated with a message. They are simply stored with a randomlly generated name (64bits in hex notation)

---

## Graphics

Lorem ipsum dolor sit amete

### messages

Graphic style is exagonal for the messages

```
  /----------------\
 /  Example message \
 \  Example message /
  \----------------/
```

