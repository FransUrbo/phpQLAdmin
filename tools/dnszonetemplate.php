<?php
// Create a DNS zone file
// $Id: dnszonetemplate.php,v 1.4 2004-02-12 14:04:52 turbo Exp $
session_start();
require("./include/pql_config.inc");
require("./include/pql_bind9.inc");

$zone = pql_bind9_get_zone($_pql->ldap_linkid, $domain, $defaultdomain);
if(is_array($zone)) {
    // We have a zone, fill in some variables used in the output
    $date = $zone[$defaultdomain]["@"]["SOA"]["SERIAL"];

    for($i=0; $zone[$defaultdomain]["@"]["NS"][$i]; $i++) 
      $nameservers[] = pql_maybe_idna_encode($zone[$defaultdomain]["@"]["NS"][$i]);

    if(is_array($zone[$defaultdomain]["@"]["MX"])) {
	foreach($zone[$defaultdomain]["@"]["MX"] as $mx) {
	    $tmp = split(' ', $mx);
	    $mailservers[$tmp[0]] = $tmp[1];
	}
    } else {
	$tmp = split(' ', $zone[$defaultdomain]["@"]["MX"]);
	$mailservers[$tmp[1]] = $tmp[0];
    }
    
    $admin   = pql_maybe_idna_encode($zone[$defaultdomain]["@"]["SOA"]["ADMIN"]);

    $refresh = $zone[$defaultdomain]["@"]["SOA"]["REFRESH"];
    $retry   = $zone[$defaultdomain]["@"]["SOA"]["RETRY"];
    $expire  = $zone[$defaultdomain]["@"]["SOA"]["EXPIRE"];
    $negttl  = $zone[$defaultdomain]["@"]["SOA"]["TTL"];
} else {
    // Create a serial number for zone
    $date = date("Ymd01");
    
    $res = getmxrr($defaultdomain, $rec, $weight);
    if(count($weight)) {
	asort($weight);
    } else {
	// Domain not registered in DNS, make up values
	$rec = array('<b>REPLACE_WITH_IP_TO_MAILSERVER</b>');
	$weight = array(10);
    }
    
    // Get Primary IP address
    if(checkdnsrr($defaultdomain.'.', 'NS')) {
	$primaryip = gethostbyname($defaultdomain.'.');
    }
    
    $nameservers = array('ns1.'.$defaultdomain.'.');
    $mailservers['mail.'.$defaultdomain.'.'] = '10';

    $admin   = "registry.".$defaultdomain.".";
    $refresh = '10800';
    $retry   = '3600';
    $expire  = '604800';
    $negttl  = '3600';

    $hosts['ns1']  = '<b>REPLACE_WITH_IP_TO_NAME_SERVER</b>';
    $hosts['www']  = '<b>REPLACE_WITH_IP_TO_WEB_SERVER</b>';
    $hosts['mail'] = '<b>REPLACE_WITH_IP_TO_MAIL_SERVER</b>';
}

if(!$primaryip) {
    $primaryip = '<b>REPLACE_WITH_IP_TO_DOMAIN</b>';
}

// Create a template (empty) zone file
$dnsparts = split('\.', $defaultdomain);
for($i=0; $dnsparts[$i]; $i++) {
    $origin = $dnsparts[$i];
}
    
// Just incase we haven't defined the TLD for this domain
// we must have the top origin (dot).
if($origin == $defaultdomain) {
    $origin = '';
}
?>
<pre>
; LDAP DN: <?="'ou=DNS,".pql_maybe_decode($domain)."'\n"?>
;
$ORIGIN <?=$origin?>.
<?=$defaultdomain?>	604800	IN	SOA	<?=$nameservers[0]?> <?=$admin?> (
<?php printf("%46d", $date);?>  ; Serial number
<?php printf("%46d", $refresh);?>  ; Refresh
<?php printf("%46d", $retry);?>  ; Retry
<?php printf("%46d", $expire);?>  ; Expire
<?php printf("%46d", $negttl);?>) ; Negative Cache TTL

			<?=$retry?>	IN	A	<?=$primaryip."\n"?>
<?php foreach($nameservers as $ns) { ?>
			<?=$retry?>	IN	NS	<?=$ns."\n"?>
<?php } ?>
<?php foreach($mailservers as $key => $prio) { ?>
			<?=$retry?>	IN	MX	<?=$prio?> <?=$key."\n"?>
<?php }?>

$ORIGIN <?=$defaultdomain?>.
<?php
if(is_array($zone[$defaultdomain])) {
    foreach($zone[$defaultdomain] as $data) {
	if($data['HOST'] != '@') {
	    printf("%-20s %8d	IN	", $data['HOST'], $data['TTL']);
	    if($data['CNAME']) {
		printf("%-6s	%s\n", 'CNAME', $data['CNAME']);
	    } elseif($data['A']) {
		printf("%-6s	%s\n", 'A', $data['A']);
	    } elseif($data['SRV']) {
		printf("%-6s	%s\n", 'SRV', $data['SRV']);
	    } elseif($data['TXT']) {
		printf("%-6s	%s\n", "TXT", $data['TXT']);
	    }
	}
    }
}
?>
</pre>
</body>
</html>
