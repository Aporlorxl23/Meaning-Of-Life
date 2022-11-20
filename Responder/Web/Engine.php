<?php

function generateRandomString($length = 10) {
    $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
    $charactersLength = strlen($characters);
    $randomString = '';
    for ($i = 0; $i < $length; $i++) {
        $randomString .= $characters[rand(0, $charactersLength - 1)];
    }
    return $randomString;
}

if (isset($_GET['ID']) && isset($_GET['Create'])) {
        $ID = escapeshellcmd($_GET['ID']);
        $EngineCode=<<<EOD
<?php
        require 'Manager.php';

        \$Ip = getUserIP()."\n";

        \$fn = fopen("$ID","r");

        while(! feof(\$fn))  {
                \$result = trim(fgets(\$fn));

                if (trim(\$result) !== trim(\$Ip)) {

                        \$fp = fopen('$ID', 'a');
                        fwrite(\$fp, \$Ip);
                        fclose(\$fp);
                }
        }

        fclose(\$fn);


?>
EOD;
if (strpos(urldecode($_GET['ID']), '..') !== false)
                die();
        file_put_contents("$ID","");
        file_put_contents("$ID.php", $EngineCode);
        echo "Success!";
}  else if (isset($_GET['ID']) && isset($_GET['Delete'])) {
        if (strpos(urldecode($_GET['ID']), '..') !== false)
                die();
        unlink(escapeshellcmd($_GET['ID']).".php");
        rename(escapeshellcmd($_GET['ID']),escapeshellcmd($_GET['ID'])."_".generateRandomString());

} else if (isset($_GET['ID'])) {
        if (strpos(urldecode($_GET['ID']), '..') !== false)
                die();
        echo json_encode(explode("\n",trim(file_get_contents(escapeshellcmd($_GET['ID'])))));

}
