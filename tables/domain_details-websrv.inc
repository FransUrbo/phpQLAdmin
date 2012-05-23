<?php
// View and manage web servers accessible from this domain/branch.
// $Id: domain_details-websrv.inc,v 1.24 2007-02-15 12:08:16 turbo Exp $

// This view is different from 'host_details-websrv.inc' in the way that
// domain/branch (admins etc) is/should be more interested in the virtual
// host he/she owns, than what (physical) server and webserver container
// the virtual host runs on.
// The idea is to only show the virtual hosts, and then have (if running
// in advanced mode) a list of webserver containers this virtual host is
// located on...
// The domain_details-websrv*.inc files are _very_ similar to the host
// details counterpart (I actually took the host details pages, saved
// them as a domain view file and then modified them to fit this different
// layout...

// {{{ Include Webserver API etc
require($_SESSION["path"]."/include/pql_websrv.inc");
// }}}

// {{{ Get all domain names - primary and additional - this domain 'own'
$domain_additional = $_pql->get_attribute($_REQUEST["domain"], pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME"));
if(is_array($domain_additional)) {
  $domains = $domain_additional;
} elseif($domain_additional) {
  $domains = array($domain_additional);
}

$domain_default    = $_pql->get_attribute($_REQUEST["domain"], pql_get_define("PQL_ATTR_DEFAULTDOMAIN"));
if($domain_default) {
  $domains[] = $domain_default;
}
// }}}

// {{{ Get all virtual hosts which this domain/branch 'own'.
$filter = "(|";
for($i=0; $domains[$i]; $i++) {
  $filter .= "(".pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME")."=*".$domains[$i].")";
}
$filter .= ")";
$virt_hosts = $_pql->get_dn($_SESSION["USER_SEARCH_DN_CTR"], $filter);
// }}}

// {{{ Setup a virtual host array
if(is_array($virt_hosts)) {
  for($i=0; $virt_hosts[$i]; $i++) {
	$dn_parts = ldap_explode_dn($virt_hosts[$i], 0);

	$virt_host = $dn_parts[0];
	$container = $dn_parts[1];

	// Remove the attribute from the values
	$virt_host = preg_replace('/.*=/', '', $virt_host);
	$container = preg_replace('/.*=/', '', $container);

	// Setup an array, ordered firstly on virtual host, then on web container
	$VIRTHOSTS[$virt_host][$container] = $virt_hosts[$i];
  }
} else {
  // Simplest way to avoid PHP warnings: Invalid argument supplied for foreach()
  $VIRTHOSTS = array();
  $host_dn = array();
}
// }}}

// {{{ Figure out what part of the virtual hosts array to use
if(!@$_REQUEST["host"]) {
  // Take the first virtual host as default one.
  foreach($VIRTHOSTS as $_REQUEST["host"] => $host_dn) break;
} else {
  // Extract only the specific virtual host part of the VIRTHOSTS array.
  // This so that the next foreach() works in all cases...
  $virt_host_name = $_REQUEST["host"];
  $host_dn = $VIRTHOSTS[$virt_host_name];
}

// For internal use - we MUST know the name of the webserver container
// of this host - this so that we can retreive the information about
// this virtual host.
foreach($host_dn as $container => $virt_host_dn) break;
// }}}

if(count($VIRTHOSTS)) {
  // {{{ Setup the server buttons
?>
  <table cellspacing="0" border="0" width="50%" cellpadding="0">
    <th colspan="3" align="left">
<?php foreach($VIRTHOSTS as $name => $dn) {
		if($name == $_REQUEST["host"]) {
		  $mark = "&mark=yes";
		} else {
		  unset($mark);
		}
?>
    <a href="<?php echo $_SERVER["PHP_SELF"]?>?rootdn=<?php echo urlencode($_REQUEST["rootdn"])?>&domain=<?php echo urlencode($_REQUEST["domain"])?>&view=<?php echo $_REQUEST["view"]?>&host=<?php echo urlencode($name)?>&ref=<?php echo $_REQUEST["ref"]?>"><img alt="// <?php echo $name?> \\" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?php echo $name?><?php echo $mark?>"></a>
<?php } ?>
    </th>
  </table>

  <br>
<?php
// }}}

  // {{{ Setup the virtual host directory buttons
  // Find any virtual host locations for this virtual host
  $locations = pql_websrv_get_virtual_host_directories('', '', $virt_host_dn);

  if(count($locations) >= 1) {
?>
  <table cellspacing="0" border="0" width="50%" cellpadding="0">
    <th colspan="3" align="left">
<?php foreach($locations as $virt_host_location => $data) {
		if($virt_host_location == $_REQUEST["hostdir"]) {
		  $mark = "&mark=yes";
		} else {
		  unset($mark);
		}
?>
    <a href="<?php echo $_SERVER["PHP_SELF"]?>?rootdn=<?php echo urlencode($_REQUEST["rootdn"])?>&domain=<?php echo urlencode($_REQUEST["domain"])?>&view=<?php echo $_REQUEST["view"]?>&host=<?php echo urlencode($_REQUEST['host'])?>&hostdir=<?php echo $virt_host_location?>&ref=<?php echo $_REQUEST["ref"]?>"><img alt="// <?php echo $virt_host_location?> \\" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?php echo $virt_host_location?><?php echo $mark?>"></a>
<?php } ?>
    </th>
  </table>

  <br>
<?php
  }
// }}}

  // {{{ Extract information about this virtual host (or virtual host location)
  if(@$_REQUEST["hostdir"])
	// Called with a virtual host location. Add that at the beginning of the virtual host DN.
	$virt_host_dn = pql_get_define("PQL_ATTR_WEBSRV_SRV_IP").'='.$_REQUEST["hostdir"].','.$virt_host_dn;

  $virt_host_object = pql_websrv_get_virtual_host($virt_host_dn);
// }}}

  // {{{ Include tables/host_details-websrv_virt{dir,host}.inc
?>

  <table cellspacing="0" cellpadding="3" border="0">
<?php
if($_REQUEST["hostdir"]) {
  require($_SESSION["path"]."/tables/domain_details-websrv_virtdir.inc");
} else {
  require($_SESSION["path"]."/tables/domain_details-websrv_virthost.inc");
}
?>
  </table>

<?php
// }}}
} else {
  echo $LANG->_("No virtual hosts configured for this branch");
}

// {{{ If Branch/Domain or SUPER admin - Show 'Add virtual host' link
if((pql_validate_administrator($_REQUEST["domain"], pql_get_define("PQL_ATTR_ADMINISTRATOR"), $_SESSION["USER_DN"]) or
	pql_validate_administrator($_REQUEST["domain"], pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV"), $_SESSION["USER_DN"]) or
	$_SESSION["ALLOW_BRANCH_CREATE"]) and $_SESSION["ADVANCED_MODE"])
{
?>
  <br>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?php echo $LANG->_('Actions')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="websrv_add.php?rootdn=<?php echo urlencode($_REQUEST["rootdn"])?>&domain=<?php echo urlencode($_REQUEST["domain"])?>&view=<?php echo $_REQUEST["view"]?>&ref=domain"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('virtual host'))); ?></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
