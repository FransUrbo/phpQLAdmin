<?php
// shows results of search
// $Id: search.php,v 2.43 2007-09-29 21:15:10 turbo Exp $
//
// {{{ Includes
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
include($_SESSION["path"]."/header.html");
// }}}

// {{{ Print status message, if one is available
if(isset($msg)) {
    pql_format_status_msg($msg);
}
// }}}

// {{{ Reload navigation bar if needed
if(isset($_REQUEST["rlnb"]) and pql_get_define("PQL_CONF_AUTO_RELOAD")) {
?>
  <script src="tools/frames.js" type="text/javascript" language="javascript1.2"></script>
  <script language="JavaScript1.2"><!--
    // reload navigation frame
    refreshFrames();
  //--></script>

<?php
}
// }}}
?>
  <span class="title1"><?php echo $LANG->_('Search Results')?></span>
  <br><br>

<?php
$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

// {{{ Test for submission of variables
if(empty($_REQUEST["attribute"]) or empty($_REQUEST["filter_type"]) or empty($_REQUEST["search_string"])) {
    // Invalid form submission
    $msg = urlencode($LANG->_('You have to provide a value to search for'));
    pql_header("home.php?msg=$msg", 1);
}
// }}}

if($_REQUEST["attribute"] == sprintf("%s", pql_get_define("PQL_ATTR_MAILHOST"))) {
	// IDNA decode the FQDN
	$_REQUEST["search_string"] = pql_maybe_idna_encode($_REQUEST["search_string"]);

	if(($_REQUEST["filter_type"] != 'not') && ($_REQUEST["filter_type"] != 'is'))
	  // We must force an 'is', since it's not possible to do a substring match
	  $_REQUEST["filter_type"] = 'is';
}

// {{{ Make filter to comply with filter_type and search_string
$filter = "";
if($_REQUEST["attribute"] != pql_get_define("PQL_ATTR_ARECORD")) {
switch($_REQUEST["filter_type"]) {
  case "is":
	if($_REQUEST["attribute"] == sprintf("%s", pql_get_define("PQL_ATTR_MAIL"))) {
		$filter  = '(|('.pql_get_define("PQL_ATTR_MAIL").'='.$_REQUEST["search_string"];
		$filter .= ')('.pql_get_define("PQL_ATTR_MAILALTERNATE").'='.$_REQUEST["search_string"];
		$filter .= '))';
	} else
	  $filter = $_REQUEST["attribute"] . "=" . $_REQUEST["search_string"];
    break;
  case "ends_with":
	if($_REQUEST["attribute"] == sprintf("%s", pql_get_define("PQL_ATTR_MAIL"))) {
		$filter  = '(|('.pql_get_define("PQL_ATTR_MAIL").'=*'.$_REQUEST["search_string"];
		$filter .= ')('.pql_get_define("PQL_ATTR_MAILALTERNATE").'=*'.$_REQUEST["search_string"];
		$filter .= '))';
	} else
	  $filter = $_REQUEST["attribute"] . "=*" . $_REQUEST["search_string"];
    break;
  case "starts_with":
	if($_REQUEST["attribute"] == sprintf("%s", pql_get_define("PQL_ATTR_MAIL"))) {
		$filter  = '(|('.pql_get_define("PQL_ATTR_MAIL").'='.$_REQUEST["search_string"].'*';
		$filter .= ')('.pql_get_define("PQL_ATTR_MAILALTERNATE").'='.$_REQUEST["search_string"].'*';
		$filter .= '))';
	} else
	  $filter = $_REQUEST["attribute"] . "=" . $_REQUEST["search_string"] . "*";
    break;
  case "not":
	if($_REQUEST["attribute"] == sprintf("%s", pql_get_define("PQL_ATTR_MAIL"))) {
		$filter  = '(&(!(('.pql_get_define("PQL_ATTR_MAIL").'='.$_REQUEST["search_string"].'*';
		$filter .= ')('.pql_get_define("PQL_ATTR_MAILALTERNATE").'='.$_REQUEST["search_string"].'*';
		$filter .= '))))';
	} else {
	  $filter  = '(&(|('.pql_get_define("PQL_ATTR_MAIL").'=*)('.pql_get_define("PQL_ATTR_MAILALTERNATE").'=*))';
	  $filter .= '(!('.$_REQUEST["attribute"] . "=" . $_REQUEST["search_string"];
	  if($_REQUEST["attribute"] != pql_get_define("PQL_ATTR_MAILHOST"))
		$filter .= '*';
	  $filter .= ')))';
	}
	break;
  default:
	if($_REQUEST["attribute"] == sprintf("%s", pql_get_define("PQL_ATTR_MAIL"))) {
		$filter  = '(|('.pql_get_define("PQL_ATTR_MAIL").'=*'.$_REQUEST["search_string"].'*';
		$filter .= ')('.pql_get_define("PQL_ATTR_MAILALTERNATE").'=*'.$_REQUEST["search_string"].'*';
		$filter .= '))';
	} else
	  $filter = $_REQUEST["attribute"] . "=*" . $_REQUEST["search_string"] . "*";
    break;
}
} else {
  // Looking for an IP address - Can only search with 'is'!
  $filter = $_REQUEST["attribute"] . "=" . $_REQUEST["search_string"];
}
// }}}

if(!$_SESSION["SINGLE_USER"]) {
	// {{{ Admin of some sort - look in the whole database for a user that matches filter
	foreach($_SESSION["BASE_DN"] as $dn) {
		if($_REQUEST["debug"]) {
?>
  <table cellspacing="0" cellpadding="3" border="0">
	<th colspan="4" align="left"><h5><u><?php echo $LANG->_('Search filter debugging')?></u></h5>
      <tr>
        <td>DN:</td>
        <td><b><?php echo $dn?></b></td>
      <tr>

      <tr>
        <td>Filter:</td>
        <td><b><?php echo $filter?></b></td>
      <tr>
    </th>
  </table>
  <p>
<?php	}

		$enrs = $_pql->get_dn($dn, $filter, 'SUBTREE');
		if(!empty($enrs)) {
		  for($i=0; $i < count($enrs); $i++) {
			$is_group = 0;
			
			// Check if this object is a (posix)Group object.
			$ocs = $_pql->get_attribute($enrs[$i], pql_get_define("PQL_ATTR_OBJECTCLASS"));
			for($j=0; $j < count($ocs); $j++) {
				if(preg_match('/group/i', $ocs[$j]))
				  $is_group = 1;
			}
			
			if(!$is_group)
			  // It's NOT a (posix)Group object, show it in the list...
			  $entries[] = $enrs[$i];
		}
		} else
		  $entries = array();
	}
// }}}
} else {
	// {{{ Single user - only look in the same branch as the user is located in
	$dn = '';

	// Get branch for user
    $dnparts = ldap_explode_dn($_SESSION["USER_DN"], 0);
	for($i=1; $i < count($dnparts); $i++) {
		$dn .= $dnparts[$i];
		if($dnparts[$i+1])
		  $dn .= ",";
	}

	// Search for the user below this DN
	$enrs = $_pql->search($dn, $filter);
	if(!empty($enrs)) {
	  for($i=0; $i < count($enrs); $i++) {
		$is_group = 0;
		
		// Check if this object is a (posix)Group object.
		$ocs = $_pql->get_attribute($enrs[$i], pql_get_define("PQL_ATTR_OBJECTCLASS"));
		for($j=0; $j < count($ocs); $j++) {
			if(preg_match('/group/i', $ocs[$j]))
			  $is_group = 1;
		}
		
		if(!$is_group)
		  // It's NOT a (posix)Group object, show it in the list...
		  $entries[] = $enrs[$i];
	}
	} else
	  $entries = array();
// }}}
}
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="4" align="left"><?php echo $LANG->_('Objects found')?>: <?php echo count($entries); ?></th>
<?php if(is_array($entries)) {
		if(($_REQUEST["attribute"] != pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")) and
		   ($_REQUEST["attribute"] != pql_get_define("PQL_ATTR_ARECORD")))
		{
		  // {{{ NOT DNS hostname
?>
      <tr>
        <td class="title"><?php echo $LANG->_('User')?></td>
        <td class="title"><?php echo $LANG->_('Username')?></td>
        <td class="title"><?php echo $LANG->_('Email')?></td>
        <td class="title"><?php echo $LANG->_('Status')?></td>
        <td class="title"><?php echo $LANG->_('Options')?></td>
      </tr>
<?php
// }}}
		} else {
		  // {{{ DNS Hostname, A/Cname records
?>
      <tr>
        <td class="title"><?php echo $LANG->_('FQDN')?></td>
        <td class="title"><?php echo $LANG->_('Target')?></td>
        <td class="title"><?php echo $LANG->_('Domain branch')?></td>
        <td class="title"><?php echo $LANG->_('Options')?></td>
      </tr>
<?php
// }}}
		}

		asort($entries);
		foreach($entries as $entry) {
		  $rootdn = pql_get_rootdn($entry, 'search.php');
		  $entry  = urlencode($entry);

		  if(($_REQUEST["attribute"] != pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")) and
			 ($_REQUEST["attribute"] != pql_get_define("PQL_ATTR_ARECORD"))) {
			// {{{ NOT DNS hostname
			$uid    = $_pql->get_attribute($entry, pql_get_define("PQL_ATTR_UID"));

			// DLW: I think displayname would be a better choice.
			$cn     = $_pql->get_attribute($entry, pql_get_define("PQL_ATTR_CN"));
			if(is_array($cn)) $cn = $cn[0];
			$mail   = $_pql->get_attribute($entry, pql_get_define("PQL_ATTR_MAIL"));
			
			$status = $_pql->get_attribute($entry, pql_get_define("PQL_ATTR_ISACTIVE"));
			$status = pql_ldap_accountstatus($status);
// }}}
		  } else {
			// {{{ Look for a DNS hostname or IP address
			$host   = $_pql->get_attribute($entry, pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME"));
			$ip     = $_pql->get_attribute($entry, pql_get_define("PQL_ATTR_ARECORD"));

			if($_REQUEST["attribute"] == sprintf("%s", pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME"))) {
			  // Looking for hostname - special case regarding IP
			  if(empty($ip)) {
				$ip   = $_pql->get_attribute($entry, pql_get_define("PQL_ATTR_CNAMERECORD"));
				if(empty($ip))
				  $ip = '<b>unknown</b>';
			  }

			  // Add the domain name to the hostname
			  $host .= $dns_domain_name;
			} elseif($_REQUEST["attribute"] == sprintf("%s", pql_get_define("PQL_ATTR_ARECORD"))) {
			  // Looking for IP address - special case regarding hostname
			  if($host == '@')
				// The SOA -> Replace with zoneName
				$host   = $_pql->get_attribute($entry, pql_get_define("PQL_ATTR_ZONENAME"));
			  else
				// This is a hostname. Add the domainname to the end to create a FQDN.
				$host  .= '.'.$dns_domain_name;
			}

			if(is_array($ip))
			  $ip = '<b>round robin</b>';
			
			// Get the zone name
			$dns_domain_name = $_pql->get_attribute($entry, pql_get_define("PQL_ATTR_ZONENAME"));
			$dns_domain_name = urlencode($dns_domain_name);

			if(empty($domain)) {
			  // Try to figure out what domain this zone belongs to
			  // 1. Find the SOA record
			  // ldapsearch -LLL -b c=se '(&(zoneName=bayour.com)(relativeDomainName=@))'
			  $filter = '(&('.pql_get_define("PQL_ATTR_ZONENAME").'='.$dns_domain_name.')('.pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME").'=@))';
			  $soa    = $_pql->get_dn($rootdn, $filter, 'SUBTREE');
			  if(!empty($soa))
				$soa  = $soa[0];
			  
			  // soa => 'dNSTTL=3600 relativeDomainName=@,dc=bayour,dc=com,ou=DNS,o=Bayour.COM,c=SE'
			  if(pql_get_define("PQL_CONF_SUBTREE_BIND9")) {
				$soa = preg_replace("/.*".lc(pql_get_define("PQL_CONF_SUBTREE_BIND9")).",/", "", $soa);
			  } else {
				//  Remove the SOA part of the DN. ldap_explode_dn() don't seem to like it!?!?
				$soa = preg_replace("/.*@,/", "", $soa);
				// soa => 'dc=bayour,dc=com,ou=DNS,o=Bayour.COM,c=SE'
			  }

			  // 2. Go two steps up, that should be the domain DN (also remove the PQL_CONF_SUBTREE_BIND9 part)
			  //    => o=Bayour.COM,c=SE
			  $dnparts = ldap_explode_dn($soa, 0);
			  
			  // 3. Put together a DN based on $dnparts
			  for($i=0; $i < $dnparts["count"]; $i++) {
				$domain .= $dnparts[$i];
				if($dnparts[$i+1])
				  $domain .= ",";
			  }
			  $domain = urlencode($domain);
			}
// }}}
		  }

		  if(($_REQUEST["attribute"] != pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")) and
			 ($_REQUEST["attribute"] != pql_get_define("PQL_ATTR_ARECORD")))
		  {
			// {{{ NOT DNS hostname
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="<?php echo $HREF?>" target="_new"><?php echo $cn?></a></td>
        <td><?php echo $uid?></td>
        <td><?php echo $mail?></td>
        <td><?php echo $status?></td>
        <td>
          <a href="user_detail.php?rootdn=<?php echo $rootdn?>&domain=<?php echo $_REQUEST["domain"]?>&user=<?php echo urlencode($entry)?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo $LANG->_('Change user data')?>" border="0"></a>
          &nbsp;
          <a href="user_del.php?rootdn=<?php echo $rootdn?>&domain=<?php echo $_REQUEST["domain"]?>&user=<?php echo urlencode($entry); ?>"><img src="images/del.png" width="12" height="12" alt="<?php echo $LANG->_('Delete user')?>" border="0"></a>
        </td>
      </tr>
<?php
// }}}
		  } else {
			// {{{ DNS Hostname
?>
      <tr class="<?php pql_format_table(); ?>">
<?php		if($_REQUEST["attribute"] == sprintf("%s", pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME"))) { ?>
        <td><?php echo $host?></td>
        <td><?php echo $ip?></td>
<?php		} elseif($_REQUEST["attribute"] == sprintf("%s", pql_get_define("PQL_ATTR_ARECORD"))) { ?>
        <td><?php echo $ip?></td>
        <td><?php echo $host?></td>
<?php		} ?>
        <td><a href="domain_detail.php?rootdn=<?php echo $rootdn?>&domain=<?php echo $domain?>&view=dnszone&dns_domain_name=<?php echo $dns_domain_name?>"><?php echo urldecode($domain)?></td>
        <td align="right">
          <a href="bind9_edit_attributes.php?rootdn=<?php echo $rootdn?>&domain=<?php echo $domain?>&action=modify&rdn=<?php echo $entry?>&view=dnszone&dns_domain_name=<?php echo $dns_domain_name?>"><img src="images/edit.png" width="12" height="12" alt="<?php echo $LANG->_('Change host data')?>" border="0"></a>
          &nbsp;
          <a href="bind9_edit_attributes.php?rootdn=<?php echo $rootdn?>&domain=<?php echo $domain?>&action=del&rdn=<?php echo $entry?>&view=dnszone&dns_domain_name=<?php echo $dns_domain_name?>"><img src="images/del.png" width="12" height="12" alt="<?php echo $LANG->_('Delete host')?>" border="0"></a>
        </td>
      </tr>
<?php
// }}}
		  }

		  unset($domain);
		} // END: foreach entries
	  } else {
		// {{{ No users registred
?>
      <tr class="<?php pql_format_table(); ?>">
        <td colspan="5"><?php echo $LANG->_('No users found')?></td>
      </tr>
<?php
// }}}
	  }

	  // {{{ Table and file end etc
?>
    </table>
  </body>
</html>
<?php
pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
// }}}
?>
