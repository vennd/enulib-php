<?php

// Requires the following extensions to be uncommented from php.ini:
// extension=php_curl.dll
// extension=php_openssl.dll

include "enulib.inc";

$enulib = new enuLib();

// Create a unique payment identifier that can be used to query the status of the payment later
$paymentId = bin2hex(openssl_random_pseudo_bytes(10, $cstrong));

// Create payment
$result = $enulib->createPayment('1DYucjPoEVzu7FMnwFVpKcvP9wo9ZYeF7u', 1200000000, 'NEKO', $paymentId, 1500);

if ($result{'code'} == 0) {
    printf("Waiting 10 seconds before querying the payment (or you can poll with getPayment() until status = 'complete')\n");
    sleep(10);

// Get full details about the payment from the enu API
    $paymentStatus = $enulib->getPayment($paymentId);
    print_r($paymentStatus);
}
else {
    printf("Failed to create payment\n");
    print($result);
}

?>