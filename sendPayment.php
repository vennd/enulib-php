<?php

// Requires the following extensions to be uncommented from php.ini:
// extension=php_curl.dll
// extension=php_openssl.dll

include "enulib.inc";

$destination_address = '1DYucjPoEVzu7FMnwFVpKcvP9wo9ZYeF7u';
$amount = 1200000000;
$asset = 'NEKO';
$txFee = 1500;

$enulib = new enuLib();

// Create a unique payment identifier that can be used to query the status of the payment later
$paymentId = bin2hex(openssl_random_pseudo_bytes(10, $cstrong));

// Create payment which sends to the given address, the given amount of the asset. The payment id is a unique identifier which you can use later to look up the payment
// The last parameter is the miner's fee (if applicable) in satoshis
// The API will return $return_status = 0 if the call is successful
printf("Sending a payment of %d %s to address: %s with a miners fee of %d\n", $amount, $asset, $destination_address, $txFee);
$result = $enulib->createPayment($destination_address, $amount, $asset, $paymentId, $txFee, $return_status);

if ($return_status == 0) {
    printf("createPayment() successful.\n");
    printf("Waiting 10 seconds before querying the payment (or you can poll with getPayment() until status = 'complete')\n");
    sleep(10);

    // Get full details about the payment from the enu API
    $result = $enulib->getPayment($paymentId, $return_status);

    if ($return_status == 0) {
        printf("getPayment() successful.\n");
        print_r($result);
    }
    else {
        printf("getPayment() failed.\n");
        printf("Error code: %d, error description: %s.\n", $result['code'], $result['description']);
    }
}
else {
    printf("Failed to create payment.\n");
    printf("Error code: %d, error description: %s.\n", $result['code'], $result['description']);
}
?>