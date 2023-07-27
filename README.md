# Personal Server Chat (PSC)

A simple client-server encrypted chat built on top TCP for personal use and small groups

## The Communication is based on a binary custom protocol described here

### General requirements

#### Who is this?

-   Who is doing the action (user or server)
    -   UUID 32bits of random bits given by the server to the client. Representable in 8 hex digits
        -   The servers check that no one in the server has the same UUID. obv
        -   32bits allow for 2^32 4 bln UUID, more than enough
    -   The server is UUID 00...00, unmistakable

#### When did it happen?

-   Time of the message being sent
    -   GMT timestamp of the message the moment the button is pressed formatted following the RFC 1123
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
        -   Res only, if you also need the binary resource

#### The actual body

-   The Body of the message

    -   The text plus some referencies to media and formatting

#### Version

-   Protocol Version
    -   4bits.4bits, only major and minor from 0.0 to 16.16
        -   1.0 for now

#### How much is being transferred

-   Content lenght in Bytes excluding the header
    -   Use remaining size in the header, 21 bits, 2097151, max 2MiB data

### Bit map

```
    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F
00 |     VM    |     Vm    | MM  | V|                            length                            |
20 |                                             UUID                                              |
40 |                                           timestamp                                           |
60 |                                             Body                                              |
...|                                                                                               |

VM => Version Major
Vm => Version Minor

M M => 00 GET
       01 POST
       10 PATCH

V => 0 TEXT
     1 BIN

length => Lenght of the body in size

timestamp => 32bit unigned int unix timestamp

UUID => Unique User ID

Body => text or binary data

```

### Endiannes

All Values should be stored in big endian 

## Network

### Header

The general header is 64bytes long and contains the minimum data i can think of that is always needed

This header does not contain sensitive data, the UUID means who is talking for the server only, so it's not needed to compress encrypt it

### Port

61970

### Protocol base

Tcp

## Graphics

dsad

### messages

Graphic style is exagonal for the messages

```
  /----------------\
 /  Example message \
 \  Example message /
  \----------------/
```

