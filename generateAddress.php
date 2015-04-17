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
$newSourceAddress = $enulib->createAddress();
printf("New source address generated: %s \n", $newSourceAddress['value'])

?>