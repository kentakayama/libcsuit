# SUIT Manifest Processor
This feature interprets the SUIT manifest binary and then executes appropriate process.  
The caller can specify the callback functions, such as fetch, copy, etc.  
There is a sample application, `suit_manifest_process`, so that you can test it.  

## How to Test: Sample Result
```
$ make -f Makefile.process
$ ./suit_manifest_process ./testfiles/suit_manifest_expAOUT.cbor

main : Read Manifest file.

main : Process Manifest file.
fetch callback : {
  uri : "http://example.com/a.out"
  dst-component-identifier : [ 'a.out' ]
  ptr : 0x7ffd238b6110 (34768)
  suit_report_t : RecPass0 RecFail1 SysPass0 SysFail0
}

main : Invoke Component.
invoke callback : {
  args : './a.out'
}
```

## Sample Input SUIT Manifest Summary
```
+-testfiles/suit_manifest_expAOUT.cbor--------+
| SUIT_Envelope {                             |
|   authentication-wrapper: [                 |
|     digest(manifest),                       |
|     COSE_Sign1(digest(manifest))            |
|   ],                                        |
|   manifest: {                               |
|     common: {                               |
|       components: [                         |
|         ['a.out']                           |
|       ]                                     |
|     },                                      |
|     install: [                              |
|       override-parameters: {                |
|         uri: "http://example.con/file.bin", |
|         image-size: 34768,                  |
|         image-digest: digest(image)         |
|       },                                    |
|       directive-fetch,                      |
|       condition-image-match                 |
|     ],                                      |
|     invoke: [                               |
|       set-parameters: {                     |
|         invole-args: './a.out'              |
|       },                                    |
|       directive-invoke                      |
|     ]                                       |
|   }                                         |
| }                                           |
+---------------------------------------------+
```

## Program Flow in Pseudocode
```
+-examples/suit_manifest_process_main.c-+
| main() {                              |
|   keys = prepare_keys();              |
|   callbacks.fetch = fetch_callback;   |
|   m = get_manifest();                 |    +-libcsuit---------------------------------------+
|   suit_process_envelope(keys, m);     |===>| suit_process_envelope(keys, m) {               |
| }                                     |    |   t = check_digest_and_extract(keys, m);       |
|                                       |    |   install(m.install) {                         |
|                                       |    |     p = extract_paraeters(m.install)           |
| fetch_callback(uri, buf, report) {    |<===|     err = fetch_callback(m.uri, buf, m.report);|
|   return get_image(uri, buf);         |===>|                                                |
| }                                     |    |   }                                            |
|                                       |    |   invoke(m.invoke) {                           |
|                                       |    |     p = extract_parameters(m.invoke);          |
| invoke_callback(args) {               |<===|     err = invoke_callback(p.args);             |
|   return system(args);                |===>|                                                |
| }                                     |    |   }                                            |
|                                       |    | }                                              |
|---------------------------------------+    +------------------------------------------------+
```

