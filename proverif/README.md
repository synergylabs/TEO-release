# TEO ProVerif formalization

## Reference Models

The base model template is at `teo-template.pv`.

## Compile Models

You need to install the Hydra python option parsing package (https://hydra.cc/docs/intro) in order to use `build.py`.
```bash
pip install hydra-core --upgrade
```

Then, to compile, you can set the number of users as:
```bash
python3 build.py numUsers=<x>
```

The output model will be in `teo-compiled.pv`.

## Run Verification

Assuming you've already compiled the model at `teo-compiled.pv`, you can check properties with the following command:
```bash
./verify.sh
```
