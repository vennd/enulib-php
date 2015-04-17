<?php
/**
 * Based upon: https://github.com/marinu666/PHP-btce-api
 */
 
class enuLib {
    protected $enu_api = 'https://enu.io/payment';
    protected $enu_api_address = 'https://enu.io/address';
//    protected $enu_api = 'http://localhost:8080/payment';
//    protected $enu_api_address = 'http://localhost:8080/address';

    protected $api_key;
    protected $api_secret;
    protected $nonce;
    protected $RETRY_FLAG = false;

    public function __construct() {
        $config_string = file_get_contents("enu_key.json");
        $config_json = json_decode($config_string, true);

        $this->api_key = $config_json{'key'};
        $this->api_secret = $config_json{'secret'};
        $this->nonce = time();
    }


    public function createPayment($destination_address, $amount, $asset, $payment_id, $tx_fee) {
        // generate the POST data json
        $post_data = array('destinationAddress' => $destination_address,
            'amount' => $amount,
            'asset' => $asset,
            'paymentId' => $payment_id,
            'txFee' => $tx_fee);

        // encode into json
        $post_data_json = json_encode($post_data);

        // Generate the keyed hash value to post
        $sign = hash_hmac("sha512", $post_data_json, $this->api_secret);

        // Add to the headers
        $headers = array(
            'accessKey: '.$this->api_key,
            'signature: '.$sign,
            'nonce: '.$this->nonce
        );

        // Create a CURL Handler for use
        $ch = null;
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
        curl_setopt($ch, CURLOPT_URL, $this->enu_api);
        curl_setopt($ch, CURLOPT_POSTFIELDS, $post_data_json);
        curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
        curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, true);
        curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 2);
        curl_setopt($ch, CURLOPT_CAINFO, "comodo_bundle.pem");

        // Send API Request
        $res = curl_exec($ch);

        print $res;

        // Check for failure & Clean-up curl handler
        if($res === false) {
            $e = curl_error($ch);
            curl_close($ch);
            throw new EnuAPIException('Could not get reply: '.$e);
        } else {
            curl_close($ch);
        }

        // Decode the JSON
        $result = json_decode($res, true);
        // is it valid JSON?
        if(!$result) {
            throw new EnuAPIException('Invalid data received, please make sure connection is working and requested API exists');
        }

        return $result;
    }

    public function getPayment($payment_id) {
        // generate the POST data json
        $post_data = array();

        // encode into json
        $post_data_json = json_encode($post_data);

        // Generate the keyed hash value to post
        $sign = hash_hmac("sha512", $post_data_json, $this->api_secret);

        // Add to the headers
        $headers = array(
            'accessKey: '.$this->api_key,
            'signature: '.$sign,
            'nonce: '.$this->nonce
        );

        // Create a CURL Handler for use
        $ch = null;
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
        curl_setopt($ch, CURLOPT_URL, $this->enu_api.'/'.$payment_id);
        curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
        curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, true);
        curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 2);
        curl_setopt($ch, CURLOPT_CAINFO, "comodo_bundle.pem");

        // Send API Request
        $res = curl_exec($ch);

        // Check for failure & Clean-up curl handler
        if($res === false) {
            $e = curl_error($ch);
            curl_close($ch);
            throw new EnuAPIException('Could not get reply: '.$e);
        } else {
            curl_close($ch);
        }

        // Decode the JSON
        $result = json_decode($res, true);
        // is it valid JSON?
        if(!$result) {
            throw new EnuAPIException('Invalid data received, please make sure connection is working and requested API exists');
        }

        return $result;
    }

    public function createAddress() {
        // generate the POST data json
        $post_data = array();

        // encode into json
        $post_data_json = json_encode($post_data);

        // Generate the keyed hash value to post
        $sign = hash_hmac("sha512", $post_data_json, $this->api_secret);

        // Add to the headers
        $headers = array(
            'accessKey: '.$this->api_key,
            'signature: '.$sign,
            'nonce: '.$this->nonce
        );

        // Create a CURL Handler for use
        $ch = null;
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
        curl_setopt($ch, CURLOPT_URL, $this->enu_api_address);
        curl_setopt($ch, CURLOPT_POSTFIELDS, $post_data_json);
        curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
        curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, true);
        curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 2);
        curl_setopt($ch, CURLOPT_CAINFO, "comodo_bundle.pem");

        // Send API Request
        $res = curl_exec($ch);

        print $res;

        // Check for failure & Clean-up curl handler
        if($res === false) {
            $e = curl_error($ch);
            curl_close($ch);
            throw new EnuAPIException('Could not get reply: '.$e);
        } else {
            curl_close($ch);
        }

        // Decode the JSON
        $result = json_decode($res, true);
        // is it valid JSON?
        if(!$result) {
            throw new EnuAPIException('Invalid data received, please make sure connection is working and requested API exists');
        }

        return $result;
    }
}

/**
 * Exceptions
 */
class EnuAPIException extends Exception {}
?>