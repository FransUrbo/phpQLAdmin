<?php
// Create a DNS zone file
// $Id: dnszonetemplate.php,v 1.16 2007-02-15 12:45:22 turbo Exp $
// {{{ Setup session etc
require("../include/pql_session.inc");
require("../include/pql_config.inc");
require($_SESSION["path"]."/include/pql_bind9.inc");
// }}}

$TYPES = array('CNAME', 'A', 'SRV', 'TXT', 'PTR');
$domain = $_REQUEST["domain"];
$defaultdomain = $_REQUEST["defaultdomain"];

$TYPES = array('CNAME', 'A', 'SRV', 'TXT', 'PTR');
$domain = $_REQUEST["domain"];
$defaultdomain = $_REQUEST["defaultdomain"];

$zone = pql_bind9_get_zone($_pql->ldap_linkid, $domain, $defaultdomain);
if(is_array($zone)) {
    // We have a zone, fill in some variables used in the output
    $date = $zone[$defaultdomain]["@"]["SOA"]["SERIAL"];

    for($i=0; $i < count($zone[$defaultdomain]["@"]["NS"]); $i++) 
      $nameservers[] = pql_maybe_idna_encode($zone[$defaultdomain]["@"]["NS"][$i]);
    if(!is_array($nameservers))
      // Resonable default...
      $nameservers = array('ns1.'.$defaultdomain.'.');

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
    $admin   = preg_replace('/@/', '.',  $admin);

    $refresh = $zone[$defaultdomain]["@"]["SOA"]["REFRESH"];
    $retry   = $zone[$defaultdomain]["@"]["SOA"]["RETRY"];
    $expire  = $zone[$defaultdomain]["@"]["SOA"]["EXPIRE"];
    $negttl  = $zone[$defaultdomain]["@"]["SOA"]["TTL"];

    if($zone[$defaultdomain]["@"]["A"]) {
      $primaryip = $zone[$defaultdomain]["@"]["A"];
    }
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
for($i=0; $i < count($dnsparts); $i++) {
    $origin = $dnsparts[$i];
}
    
// Just incase we haven't defined the TLD for this domain
// we must have the top origin (dot).
if($origin == $defaultdomain) {
    $origin = '';
}

// This is the domain, but with the TLD removed. For use in the SOA record.
$basedomain = eregi_replace("\.".$origin, "", $defaultdomain);
?>
    <pre>
<?php
echo "; LDAP DN: '".pql_get_define("PQL_CONF_SUBTREE_BIND9").",".pql_maybe_decode($domain)."'\n";
echo "\$ORIGIN $origin.\n";
printf("%-25s %8s	IN	SOA	%s %s. (\n", $basedomain, $negttl, $nameservers[0], $admin);
printf("%58d  ; Serial number\n", $date);
printf("%58d  ; Refresh\n", $refresh);
printf("%58d  ; Retry\n", $retry);
printf("%58d  ; Expire\n", $expire);
printf("%58d) ; Negative Cache TTL\n", $negttl);

echo "; ------------------------------\n";
if(is_array($primaryip)) {
  asort($primaryip);
  for($i=0; $primaryip[$i]; $i++)
    printf("%25s %8s IN	A	".$primaryip[$i]."\n", " ", $retry);
} else {
  printf("%25s %8s IN	A	$primaryip\n", " ", $retry);
}
foreach($nameservers as $ns) {
  printf("%25s %8s IN	NS	$ns\n", " ", $retry);
}
foreach($mailservers as $prio => $key) {
  printf("%25s %8s IN	MX	%-4s $key\n", " ", $retry, $prio);
}

echo "; ------------------------------\n";
echo "\$ORIGIN $defaultdomain.\n";
$printed_hosts = 0;
if(is_array($zone[$defaultdomain])) {
    foreach($zone[$defaultdomain] as $data) {
	if($data['HOST'] != '@') {
	  foreach($TYPES as $type) {
	    if($data[$type]) {
	      unset($records);
	      if(!is_array($data[$type]))
		$records[] = $data[$type];
	      else
		$records = $data[$type];
	      
	      foreach($records as $record)
		printf("%-25s %8d IN	%-6s	%s\n", pql_maybe_idna_encode($data['HOST']), $data['TTL'], $type, $record);
	      
	      $printed_hosts = 1;
	    }
	  }
	}
    } 
}
if(!$printed_hosts) {
    // Just for show :)
    echo "; [add your host(s) here]\n";
}

$link  = $_SESSION["URI"]."domain_detail.php?rootdn=".urlencode($_REQUEST["rootdn"]);
$link .= "&domain=".urlencode($_REQUEST["domain"])."&view=".$_REQUEST["view"];
$link .= "&dns_domain_name=$defaultdomain";
?>
    </pre>

    <form action="<?=$link?>" method="post">
      <input type="submit" value="Continue">
    </form>
  </body>
</html>
