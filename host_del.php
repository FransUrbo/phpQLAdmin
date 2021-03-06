<?php
// delete a physical host
// $Id: host_del.php,v 2.3 2007-03-14 12:20:22 turbo Exp $

// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
include($_SESSION["path"]."/header.html");
// }}}

// {{{ Find out what DN(s) to delete
$rootdn = pql_get_rootdn($_SESSION["USER_SEARCH_DN_CTR"], 'host_del.php'); $rootdn = urldecode($rootdn);
if(!$_REQUEST["dns"]) {
  if(@$_REQUEST["hostdir"]) {
	// {{{ Remove a virtual host location
	// Find the DN of this virtual host
	$virt_host = $_pql->get_dn($_REQUEST["server"], pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME")."=".$_REQUEST["virthost"]);
	if(is_array($virt_host)) {
	  $virt_host = $virt_host[0];
	  
	  // Find the DN of this virtual host location
	  $location =  $_pql->get_dn($virt_host, pql_get_define("PQL_ATTR_WEBSRV_SRV_IP")."=".$_REQUEST["hostdir"]);
	  if(is_array($location)) {
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Delete %what%'), array('what' => 'host location'))?>: <?php echo $_REQUEST["virthost"]?></span>
<?php
		$dns = array($location[0]);
	  } else
		die("Can't figure out the DN of the location '".$_REQUEST["hostdir"]."' (below virtual host '$virt_host')");
	} else
	  die("Can't figure out the DN of the virtual host '".$_REQUEST["virthost"]."' (below web server '".$_REQUEST["server"]."')");
// }}}
	
  } elseif(@$_REQUEST["virthost"]) {
	// {{{ Remove a virtual host
	// Find the DN of this virtual host
	$virt_host = $_pql->get_dn($_REQUEST["server"], pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME")."=".$_REQUEST["virthost"]);
	if(is_array($virt_host)) {
	  $virt_host = $virt_host[0];
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Delete %what%'), array('what' => 'virtual host'))?>: <?php echo $_REQUEST["virthost"]?></span>
<?php
	  
	  // Find all objects below this virtual host
	  $dns = $_pql->get_dn($virt_host, pql_get_define("PQL_ATTR_OBJECTCLASS").'=*');
	} else
	  die("Can't figure out the DN of the virtual host '".$_REQUEST["virthost"]."' (below web server '".$_REQUEST["server"]."')");
// }}}

  } elseif(@$_REQUEST["server"]) {
	// {{{ Delete a web server container
	// Get the server reference for this web server
	$server_reference = $_pql->get_attribute($_REQUEST["server"], pql_get_define("PQL_ATTR_CN"));
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Delete %what%'), array('what' => 'web server'))?>: <?php echo $server_reference?></span>
<?php
	// Find all objects below this web server
	$dns = $_pql->get_dn($_REQUEST["server"], pql_get_define("PQL_ATTR_OBJECTCLASS").'=*');
// }}}

  } elseif(@$_REQUEST["host"]) {
	// {{{ Delete a physical host
	// Get the server reference for this physical server
	$server_reference = $_pql->get_attribute($_REQUEST["host"], pql_get_define("PQL_ATTR_CN"));
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Delete %what%'), array('what' => 'physical host'))?>: <?php echo $server_reference?></span>
<?php
	// Find all objects below this physical host
	$dns = $_pql->get_dn($_REQUEST["host"], pql_get_define("PQL_ATTR_OBJECTCLASS").'=*');
// }}}
  }

  // Fake a $_REQUEST["dn_XX"] array
  for($i=0; $dns[$i]; $i++)
	$_REQUEST["dn_".$i] = $dns[$i];
  $_REQUEST["dns"] = $i-1;

  if(!pql_get_define("PQL_CONF_VERIFY_DELETE", $rootdn))
	$_REQUEST["ok"] = 'Yes';

  echo "    <p>\n";
}

if(pql_get_define("PQL_CONF_DEBUG_ME")) {
  echo "_REQUEST:";
  printr($_REQUEST);
}
// }}}

if(isset($_REQUEST["ok"])) {
  // {{{ Delete the FIRST DN recursivly
  if(pql_domain_del($_REQUEST["dn_0"], 0)) {
	$msg = urlencode(pql_complete_constant($LANG->_('Successfully removed %what%'), array('what' => $_REQUEST["dn_0"])));

	if(((!@isset($_REQUEST["del_dns_host"]) and !@$_REQUEST["replicas"]) or ($_REQUEST["del_dns_host"] == 1))) {
	   // {{{ Delete any DNS host entries that pointed to this <whatever>
	   // {{{ Separate the domainname and hostname from the FQDN
	   if($_REQUEST["virthost"])
	   $server_reference = $_REQUEST["virthost"];
	   elseif($_REQUEST["server"]) {
		 $server_reference = $_pql->get_attribute($_REQUEST["server"], pql_get_define("PQL_ATTR_CN"));
	   } elseif($_REQUEST["host"]) {
		 $server_reference = $_pql->get_attribute($_REQUEST["host"], pql_get_define("PQL_ATTR_CN"));
	   }
	   $tmp = pql_separate_host_domain($server_reference);
	   $hostname = $tmp[0]; $domainname = $tmp[1];
// }}}

	   // {{{ Find this host entry in the DNS
	   $filter = '(&('.pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")."=$hostname)(".pql_get_define("PQL_ATTR_ZONENAME")."=$domainname))";
	   $dns_entry = $_pql->get_dn($rootdn, $filter);
// }}}

	   // {{{ Delete the DNS record
	   if(@$dns_entry[0]) {
		 if($_pql->delete($dns_entry[0]))
		   $msg .= "<br>".urlencode(pql_complete_constant($LANG->_('Successfully removed %what%'), array('what' => $dns_entry[0])));
	   }
// }}}

	   $link = "home.php?msg=$msg&rlnb=2";
// }}}
	}
  } else {
	$msg  = urlencode(pql_complete_constant($LANG->_('Failed to remove %what%'), array('what' => $_REQUEST["dn_0"]))) . ":&nbsp;" . ldap_error($_pql->ldap_linkid);

	if($_REQUEST["view"] == 'action')
	  $link = "host_detail.php";
	else
	  $link = "host_detail.php?view=".$_REQUEST["view"];

	if(@$_REQUEST["host"]) {
	  if($_REQUEST["view"] == 'action')
		$link .= '?';
	  else
		$link .= '&';

	  $link .= 'host='.urlencode($_REQUEST["host"]);
	}

	if(@$_REQUEST["server"])
	  $link .= '&server='.urlencode($_REQUEST["server"]);
	if(@$_REQUEST["virthost"])
	  $link .= '&virthost='.$_REQUEST["virthost"];
	if(@$_REQUEST["hostdir"])
	  $link .= '&hostdir='.$_REQUEST["hostdir"];

	$link .= "&msg=$msg";
  }

  pql_header($link);
// }}}
} else {
  // {{{ Get confirmation and ask HOW to delete
?>
    <img src="images/info.png" width="16" height="16" border="0">
    <font color="red">
<?php
  if(pql_get_define("PQL_CONF_DEBUG_ME")) {
	echo $LANG->_('The following objects will be deleted:')."<p>";
	for($i=0; $dns[$i]; $i++) {
	  echo $dns[$i]."<br>";
	}
  } else
	echo pql_complete_constant($LANG->_('A total of %amount% objects will be deleted'), array('amount' => count($dns)))."<br>";
?>
    </font>
    <br>
    <img src="images/info.png" width="16" height="16" border="0">
<?php if(@$_REQUEST["virthost"]) { ?>
    <?php echo $LANG->_('Attention: If you delete a virtual host, all locations (if any) will be deleted too'); ?>!
<?php } elseif(@$_REQUEST["server"]) { ?>
    <?php echo $LANG->_('Attention: If you delete a web server, all virtual hosts and locations (if any) will be deleted too'); ?>!
<?php } elseif(@$_REQUEST["host"]) { ?>
    <?php echo $LANG->_('Attention: If you delete a physical host, all mail- and webservers, host ACL, sudoers, auto mount information etc, etc will be deleted too'); ?>!
<?php } ?>
    <p>
    <form action="<?php echo $_SERVER["PHP_SELF"]; ?>" method="GET">
      <?php echo $LANG->_('Delete any DNS host entries?')?>&nbsp;
      <input type="radio" name="del_dns_host" value="1"<?php if(!@$_REQUEST["replicas"]) { echo " CHECKED"; }?>>&nbsp;<?php echo $LANG->_('Yes')?>
      <input type="radio" name="del_dns_host" value="0"<?php if( @$_REQUEST["replicas"]) { echo " CHECKED"; }?>>&nbsp;<?php echo $LANG->_('No')?>
      <br>
<?php	for($i=0; $dns[$i]; $i++) { ?>
      <input type="hidden" name="dn_<?php echo $i?>" value="<?php echo urlencode($dns[$i])?>">
<?php	} ?>
      <input type="hidden" name="dns" value="<?php echo $i-1?>">

      <input type="hidden" name="host"     value="<?php echo $_REQUEST["host"]?>">
      <input type="hidden" name="server"   value="<?php echo $_REQUEST["server"]?>">
      <input type="hidden" name="virthost" value="<?php echo $_REQUEST["virthost"]?>">
      <input type="hidden" name="hostdir"  value="<?php echo $_REQUEST["hostdir"]?>">
      <input type="hidden" name="view"     value="<?php echo $_REQUEST["view"]?>">

      <?php echo $LANG->_('Are you really sure'); ?>?&nbsp;
      <input type="submit" name="ok" value="<?php echo $LANG->_('Yes'); ?>">
      <input type="button" name="back" value="<?php echo $LANG->_('No'); ?>" onClick="history.back();">
    </form>
<br>
<?php
// }}}
}

pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
