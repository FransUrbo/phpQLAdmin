<?php
// delete a domain and all users within
// $Id: unit_del.php,v 1.1 2002-12-12 06:23:19 turbo Exp $
//
require("pql.inc");
require("pql_control.inc");
?>

<html>
<head>
	<title>phpQL</title>
	<link rel="stylesheet" href="normal.css" type="text/css">
</head>

<body bgcolor="#e7e7e7" background="images/bkg.png">
<span class="title1"><?php echo pql_complete_constant(PQL_DOMAIN_DEL_TITLE, array("domain" => $domain))?></span>
<?php
	if($ok != 1){
?>
<br>
<br>
<img src="images/info.png" width="16" height="16" border="0">
<?php echo PQL_DOMAIN_DEL_WARNING; ?>
<br>
<br>
<?php echo PQL_SURE; ?>
<br>
<a href="domain_del.php?domain=<?php echo $domain; ?>&unit=<?php echo $unit; ?>&ok=1"><?php echo PQL_YES; ?></a>, <a href="javascript:history.back()"><?php echo PQL_NO; ?></a>
<br>
<?php
  } else {
  	$_pql = new pql();
		$_pql_control = new pql_control();

	// delete the unit 
	if(pql_remove_unit($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $unit)){

		// update locals if control patch is enabled
		if(pql_control_update_domains($_pql->ldap_linkid, PQL_LDAP_BASEDN, $_pql_control->ldap_linkid, PQL_LDAP_CONTROL_BASEDN)){
			// message ??
		}

		$msg = PQL_DOMAIN_DEL_OK;
		// redirect to home page
		$msg = urlencode($msg);
		header("Location: home.php?msg=$msg&rlnb=1");

  	} else {
    	$msg = PQL_DOMAIN_DEL_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
		// redirect to domain detail page
		$msg = urlencode($msg);
		header("Location: domain_detail.php?domain=$domain&unit=$unit&msg=$msg");
    }


  } // end of if
?>
</body>
</html>
