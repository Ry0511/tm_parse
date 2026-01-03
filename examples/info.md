# Binary File Layout

The binary file is made up of several sections. Some sections are purely metadata while others are
'executable' instructions with executable here meaning that a runtime can interpret and evaluate.

## File Elements

There are a few different types of elemennts in the binary file. They are broken into two categories
those used as building blocks Leaf/Mono and those that are composite made up of the building blocks
and or the composites as well.

i.e., You can encode an array of integers or an array of key-values or an array of key values where
said values are expressions which may need to be 'evaluated'.

for example the following set command will need to be evaluated:

```text
set $PC Pawn.Location Pawn.Location.Y + 500.0
```

Since we at compile time will not know anything about `$PC` all we can do is tell you in the simplest
way what should be done to compute the value and then you fill in the blanks at runtime.

The above could become something like this

```text
BeginObjectEdit
  ; for global set commands we will keep the SrcObject alive; other style set commands might not
  KeepAlive Bool True
  SrcObject MetaVar Int8 2 PC
  ; Start editing a property
  BeginPropertyEdit
    BeginArray
      Int8 2
      Str Int8 4 Pawn
      Str Int8 8 Location
    End
    ; The type of the expression is up to the runtime to figure out i.e., assigning a string to a
    ;  number you might refuse to do it
    BeginExpr
      ; By default unless told otherwise you should resolve PropertyValues based on the object being edited
      PropertyValue
        BeginArray
          Int8 3
          Str Int8 4 Pawn
          Str Int8 8 Location
          Str Int8 1 Y
        End
      Add
      Float 500.0
    End
  End
End
```

As a forewarning someone could try to do this `set obj prop "Player Y-Position is " + $PC.Pawn.Location.Y`

This should hopefully outline the nature of

#### Primitives

> note everything here is little endian

The primitives used in the binary file are:

| Type    | Size             | Description                                                             |
|---------|------------------|-------------------------------------------------------------------------|
| `Int8`  | **1 + 1**        | Signed 8-bit                                                            |
| `Int16` | **1 + 2**        | Signed 16-bit                                                           |
| `Int32` | **1 + 4**        | Signed 32-bit                                                           |
| `Int64` | **1 + 8**        | Signed 64-bit                                                           |
| `Float` | **1 + 8**        | Double Precision Float 64-bit                                           |
| `Bool`  | **1 + 1**        | Boolean `0=False`, `1=True`                                             |
| `Str`   | **1 + L + C[L]** | Length + Data i.e., `Str Int8 5 HELLO` note the lack of null terminator |

> Integers are always signed and encoded as the smallest type that fits. So a 64 bit integer can be
> encoded as an Int8 if it fits into and Int8. This means you will encounter a bytecode indicating
> the type and then the value; i.e., `0x6 0x7F` could be `Int8 127`

> Floats are always encoded as 64 bit double precision

> Strings are encoded without a null terminator and are prefixed with their size
> i.e.,`Str Int8 5 Hello` and `Str Int16 500 ...` and `Str Int32 33000 ...`

##### Array of Data

| Layout           | Remarks                                                            |
|------------------|--------------------------------------------------------------------|
| Array Begin Byte | Unique byte indicating the start of an array                       |
| Length           | Int8/16/32/64                                                      |
| Data             | The data in the array Int, Float, Bool, Str, Expr, KeyValue, Array |
| End Byte         | Byte marking the end of the array                                  |

e.g.,

```text
BeginArray
  Int8 4
  Str Int8 5 Hello
  Int8 25
  Bool 1 ; 1=True
  BeginArray
    Int8 1
    Str Int16 5000
  End
End
```

##### Key Value Structure

| Layout        | Remarks                                               |
|---------------|-------------------------------------------------------|
| Begin KV Byte | Unique byte indicating the start of a Key Value block |
| Length        | Int8/16/32/64 indicating the number of KV-Pairs       |
| Key-Values    | String key followed by value                          |
| End Byte      | Byte marking the end of the Key Value Block           |

e.g.,

```text
BeginKeyValue
  Int8 2
  Str Int8 3 key
    Str Int8 5 value
  Str Int8 4 keys
    BeginKeyValue
      Int8 1
        Str Int8 3 key
          Int8 1
    End
End
```

##### Expression

TODO: Outline this one

### File Metadata

This is the file header and contains some data to simplify sanity checking and file versioning.

| Item          | Type  | Remarks                                                    |
|---------------|-------|------------------------------------------------------------|
| Magic Number  | Int32 | Magic Number/Bytes                                         |
| File Version  | Int32 | The file format version number                             |
| SHA1 Git Hash | Str   | Git Hash SHA-1                                             |
| Date String   | Str   | Date of compilation in the form `YYYY/MM/DD HH24:MM`       |
| Content Hash  | Int32 | djb2 hash of the preceeding content bytes; unsigned 32-bit |


### Mod Definition/Metadata

