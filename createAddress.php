<?php
/**
 * Created by IntelliJ IDEA.
 * User: Jeremy
 * Date: 18/04/2015
 * Time: 1:46 AM
 */

// Requires the following extensions to be uncommented from php.ini:
// extension=php_curl.dll
// extension=php_openssl.dll

include "enulib.inc";

$enulib = new enuLib();

// Create a new source address from which payments can be made. After the new source address is generated, the default address from which payments will be made is this address
$result = $enulib->createAddress($return_status);

if ($return_status == 0) {
    printf("New source address generated: %s, public key: %s, private key: %s \n", $result['value'], $result['publicKey'], $result['privateKey']);
}
else {
    printf("Failed to generate a new address.\n");
    printf("Status code: %d\n", $return_status);
    printf("Error code: %d, error description: %s.\n", $result['code'], $result['description']);
}

?>