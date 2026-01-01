

# Program Generator

The following program:

```text
create_mod = (
  name            = "My Mod Name",
  version         = (1, 0),
  authors         = ("-Ry", "Other"),
  supported_games = ("BL1", "BL2", "TPS"),
  description     = "this is an example mod",
  license         = (name = "GPL3", url = "https://choosealicense.com/licenses/gpl-3.0/"),
)

option lower_xp_scale = (
  type=Slider,
  name="Lower Xp Scale",
  value=0.5,
  min=0.0,
  max=1.0,
)

set $PC Pawn.Location (X=35, Y=20, Z=10)
set $PC XpScale(0) (Scale=$lower_xp_scale)
```

Would translate into the following pseudo-assembly instructions:

```text

BEGIN_METADATA
  KV_BEGIN
    'name' STR "My Mod Name"
    'version'
      BEGIN_ARRAY 2
        INT 1
        INT 0
      END_ARRAY
    'authors'
      BEGIN_ARRAY 2
        STR "-Ry"
        STR "Other"
      END ARRAY
    'supported_games'
      BEGIN_ARRAY 3
        STR "BL1"
        STR "BL2"
        STR "TPS"
      END_ARRAY
    'description' STR "this is an example mod"
    'license'
      KV_BEGIN
        'name' STR "GPL3"
        'url'  STR "https://choosealicense.com/licenses/gpl-3.0/"
      KV_END
  KV_END
END

BEGIN_OPTIONS
  KV_BEGIN
    'type'  STR   "Slider"
    'name'  STR   "Lower Xp Scale"
    'value' FLOAT 0.5
    'min'   FLOAT 0.0
    'max'   FLOAT 1.0
  KV_END
END

BEGIN_SET_COMMAND

END

```
