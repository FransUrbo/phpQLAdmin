<?php
// navigation bar - controls information
// $Id: left-control.php,v 2.37 2006-12-16 12:02:09 turbo Exp $
//
// {{{ Setup session etc
require("./include/pql_session.inc");

require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/pql_control.inc");
require($_SESSION["path"]."/include/pql_control_plugins.inc");
require($_SESSION["path"]."/include/pql_websrv.inc");

require("./left-head.html");
// }}}

if($_SESSION["ALLOW_CONTROL_CREATE"] or
   (pql_get_define("PQL_CONF_CONTROL_USE") or
	pql_get_define("PQL_CONF_WEBSRV_USE") or
	pql_get_define("PQL_CONF_HOSTACL_USE") or
	pql_get_define("PQL_CONF_SUDO_USE")))
{
  // We're administrating QmailLDAP/Controls, Host ACLs, Sudoers or Webserver and
  // the user is allowed to administrate.
  $div_counter = 1;

  // Retreive all necessary web server information (including access control)
  $DATA = pql_websrv_get_data($_pql);
  
  // {{{ Server control header
?>
    <!-- Server Control -->
    <div id="el<?=$div_counter?>Parent" class="parent">
      <a class="item" href="control.php">
        <font color="black" class="heada"><b>Computers</b></font>
      </a>
    </div>

<?php
  $div_counter++;
// }}}

  // {{{ 'Add physical server/host' link
  if($_SESSION["ADVANCED_MODE"] and $_SESSION["ALLOW_BRANCH_CREATE"]) {
	// Only do this if:
	//	1. Running in a advanced mode
	//	2. User is super admin
	$host = split(';', $_SESSION["USER_HOST"]);
	
	// If it's an LDAP URI, replace "%2f" with "/" -> URLdecode
	$host[0] = urldecode($host[0]);
?>
    <!-- Add physical server link -->
    <div id="el<?=$div_counter?>Parent" class="parent">
      <a href="host_add.php"><?=$LANG->_('Add physical server/host')?></a>
    </div>

<?php
	$div_counter++;
  }
// }}}

  if(!is_array($DATA)) {
	// {{{ No physical hosts
?>
<?php if($_SESSION["opera"]) { ?>
  <div id="el<?=$div_counter?>Parent" class="parent" onclick="showhide(el<?=$div_counter?>Spn, el<?=$div_counter?>Img)">
    <img name="imEx" src="images/minus.png" border="0" alt="-" width="9" height="9" id="el<?=$div_counter?>Img">
    <font color="black" class="heada">no server hosts defined</font>
  </div>
<?php } else { ?>
  <div id="el<?=$div_counter?>Parent" class="parent">
    <img src="images/navarrow.png" width="9" height="9" border="0">
    <font color="black" class="heada">no server hosts defined</font>
  </div>
<?php }
	echo "\n"; 
// }}}
  } else {
	// {{{ Check if user is controls admin in any of the root DN's
	$controls_admin = 0;
	foreach($_pql->ldap_basedn as $dn)  {
	  $dn = pql_format_normalize_dn($dn);
	  if(pql_validate_administrator($_pql->ldap_linkid, $dn, pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS"), $_SESSION["USER_DN"]))
		$controls_admin = 1;
	}
// }}}

	foreach($DATA as $physical_dn => $physical_data) {
	  // {{{ Get physical hostname for display
?>
          <!-- Physical server: <?=$physical_dn?> -->
<?php
	  if($physical_dn == 'Global')
		$host = 'Global';
	  else
		$host = pql_get_attribute($_pql->ldap_linkid, $physical_dn, pql_get_define("PQL_ATTR_CN"));
// }}}

	  // {{{ Root of host tree
	  if($physical_dn == 'Global') {
		pql_format_tree($host, "host_detail.php?host=".urlencode($physical_dn)."&ref=global");
	  } else {
		$links = array();

		// Check if user is controls/webserver admin in any of the root DN's
		$controls_admin = 0; $websrv_admin = 0;
		foreach($_pql->ldap_basedn as $dn)  {
		  $dn = pql_format_normalize_dn($dn);

		  if(pql_validate_administrator($_pql->ldap_linkid, $dn, pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS"), $_SESSION["USER_DN"]))
			$controls_admin = 1;

		  if(pql_validate_administrator($_pql->ldap_linkid, $dn, pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV"), $_SESSION["USER_DN"]))
			$websrv_admin = 1;
		}

		if(pql_get_define("PQL_CONF_CONTROL_USE") and ($controls_admin or $_SESSION["ALLOW_BRANCH_CREATE"])) {
		  // Only do this if:
		  //	1. Mail server administration is on
		  //	2. User is global mailserver administrator
		  //	3. User is super admin
		  $new = array($LANG->_('Add mail server') => "control_add_server.php?host=".urlencode($physical_dn));
		  $links = $links + $new;
		}

		if(pql_get_define("PQL_CONF_WEBSRV_USE") and ($websrv_admin or $_SESSION["ALLOW_BRANCH_CREATE"])) {
		  // Only do this if:
		  //	1.  Mail server administration is on
		  //	2. User is global webserver administrator OR
		  //	3. User is super admin
		  $new = array($LANG->_('Add web server')  => "websrv_add.php?view=websrv&host=".urlencode($physical_dn));
		  $links = $links + $new;
		}

		if($_SESSION["ALLOW_BRANCH_CREATE"] or $websrv_admin or $controls_admin) {
		  // Only do this if:
		  //	1. User is super admin
		  //	2. User is global webserver administrator
		  //	3. User is global mailserver administrator
		  //	4. User have access to virtual hosts in any of the containers of this host		<- TODO
		  pql_format_tree($host, "host_detail.php?host=".urlencode($physical_dn)."&ref=physical", $links, 0);
		} else
		  // MUST show the physical host, even if there's no links.
		  pql_format_tree($host, '', '', 0);
	  }
// }}}

	  // {{{ For each host - get QmailLDAP/Control hosts - if enabled
	  if(pql_get_define("PQL_CONF_CONTROL_USE") and ($controls_admin or $_SESSION["ALLOW_BRANCH_CREATE"])) {
		// Only do this if:
		//	1.  Mail server administration is on
		//	2. User is controls administrator OR
		//  3. User is super admin
		if($physical_dn != 'Global')
		  $qlcs = pql_get_dn($_pql->ldap_linkid, $physical_dn, '(&(cn=*)(objectclass=qmailControl))', 'ONELEVEL');
		if(is_array($qlcs) or ($physical_dn == 'Global')) {
		  // {{{ Got QLC Object(s)
		  if($physical_dn == 'Global')
			$qlcs = array('Global'); // Fake this so that the foreach() won't complain.
		  
		  foreach($qlcs as $qlc_dn) {
			// {{{ Get server name for display
			if($qlc_dn == 'Global')
			  $qlc = 'Global';
			else
			  $qlc = pql_get_attribute($_pql->ldap_linkid, $qlc_dn, pql_get_define("PQL_ATTR_CN"));
			$qlc = pql_complete_constant($LANG->_('Mailserver - %host%'), array('host' => $qlc));
// }}}

			// Get QLC categories for this QLC object
			$cats = pql_plugin_get_cats();
			if(!is_array($cats)) {
			  // {{{ No QLC plugins
			  if($_SESSION["opera"]) {
?>
  <span id="el<?=$div_counter?>Spn" style="display:''">
<?php		  } else { ?>
  <div id="el<?=$div_counter?>Child" class="child">
<?php		  } ?>
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;
      <img src="images/navarrow.png" width="9" height="9" border="0">
      <font color="black" class="heada">no plugins defined</font>
    </nobr>
<?php		  if($_SESSION["opera"]) { ?>
  </span>
<?php		  } else { ?>
  </div>
<?php		  }
// }}}
			} else {
			  // Got QLC plugins
			  asort($cats);

			  // {{{ Setup and show the branch
			  $links = array();
			  foreach($cats as $cat) {
				$new = array($cat => "control_cat.php?mxhost=".urlencode($qlc_dn)."&cat=".urlencode($cat));
				$links = $links + $new;
			  }
			  
			  pql_format_tree($qlc, "control_detail.php?mxhost=".urlencode($qlc_dn), $links, 1);
// }}}
			} // end if is_array($cats)
		  } // end foreach($qlcs)
// }}}
		} else {
		  // {{{ No QLC Object(s)
		  if($_SESSION["opera"]) {
?>
  <span id="el<?=$div_counter?>Spn" style="display:''">
<?php	  } else { ?>
  <div id="el<?=$div_counter?>Parent" class="parent">
<?php	  } ?>
    <nobr>&nbsp;&nbsp;&nbsp;
      <img src="images/navarrow.png" width="9" height="9" border="0">
      <font color="black" class="heada">no mailserver defined</font>
    </nobr>
    <br>
<?php	  if($_SESSION["opera"]) { ?>
  </span>
<?php	  } else { ?>
  </div>
<?php	  }
// }}}
		} // end if(is_array(qlcs)
	  }
// }}}

	  // {{{ For each host - get Webserver container object(s) - if enabled
	  if(is_array($physical_data) or ($physical_dn == 'Global')) {
		// {{{ Got Web container object(s)
		foreach($physical_data as $container_dn => $virt_hosts) {
		  $links = array();

		  // {{{ Get container name for display
		  if($physical_dn == 'Global')
			$container = 'Global';
		  else
			$container = pql_get_attribute($_pql->ldap_linkid, $container_dn, pql_get_define("PQL_ATTR_CN"));
// }}}

		  // {{{ Extract the port number
		  if(ereg(':', $container))
			$port = ' - ' . $LANG->_('port') . ' ' . preg_replace('/.*:/', '', $container);
		  elseif($physical_dn == 'Global')
			$port = ' - Global';
		  else
			$port = "";
		  
		  $container = pql_complete_constant($LANG->_('Webserver%port%'), array('port' => $port));
// }}}

		  if($container_dn != 'Global') {
			if(is_array($virt_hosts)) {
			  // {{{ Go through the Virtual host(s)
			  asort($virt_hosts);

			  // {{{ Add a 'Add virtual server' link
			  if($_SESSION["ALLOW_BRANCH_CREATE"] or
				 pql_validate_administrator($_pql->ldap_linkid, $container_dn, pql_get_define("PQL_ATTR_UNIQUE_GROUP"), $_SESSION["USER_DN"]))
			  {
				// Only if:
				//	1. Super admin
				//	2. User is allowed to administrate this container (is in uniqueMember)
				//	3. User is web admin of a domain which have a virtual host in this container
				$new = array('<i>=> Add virtual server</i>' => "websrv_add.php?view=websrv&ref=ctrl&host=".urlencode($physical_dn)."&server=".urlencode($container_dn));
				$links = $links + $new;
			  }
// }}}
			  
			  foreach($virt_hosts as $virthost_dn => $virthost_name) {
				// {{{ Setup the branch
				$new = array($virthost_name => "host_detail.php?host=".urlencode($physical_dn)."&server=".urlencode($container_dn)."&virthost=".urlencode($virthost_name)."&view=websrv&ref=virtual");
				$links = $links + $new;
// }}}
			  } // end foreach virt_hosts
// }}}
			} else {
			  // {{{ No virtual hosts
			  $new = array('no virtual hosts' => '');
			  $links = $links + $new;
			  
			  // Add a 'Add virtual server' link
			  if($_SESSION["ALLOW_BRANCH_CREATE"] or
				 pql_validate_administrator($_pql->ldap_linkid, $container_dn, pql_get_define("PQL_ATTR_UNIQUE_GROUP"), $_SESSION["USER_DN"]))
			  {
				// Only if:
				//	1. Super admin
				//	2. User is allowed to administrate this container (is in uniqueMember)
				//	3. User is web admin of a domain which have a virtual host in this container
				$new = array('<i>=> Add virtual server</i>' => "websrv_add.php?view=websrv&ref=ctrl&host=".urlencode($physical_dn)."&server=".urlencode($container_dn));
				$links = $links + $new;
			  }
// }}}
			}
		  } else {
			// {{{ Fake virtual host for the Global branch
			  $links = array('no virtual hosts' => '');
// }}}
		  } // end if(container != Global)

		  // --------------------------------------------
		  pql_format_tree($container, "host_detail.php?host=".urlencode($physical_dn)."&server=".urlencode($container_dn)."&view=websrv&ref=container", $links, 1);
		} // end foreach($web_containers)
// }}}
	  } else {
		// {{{ No Web container object(s)
		if($_SESSION["opera"]) {
?>

          <span id="el<?=$div_counter?>Spn" style="display:''">
<?php	} else { ?>
          <div id="el<?=$div_counter?>Child" class="child">
<?php	} ?>
            <nobr>&nbsp;&nbsp;&nbsp;
              <img src="images/navarrow.png" width="9" height="9" border="0">
              <font color="black" class="heada">no webserver defined</font>
            </nobr>
<?php	if($_SESSION["opera"]) { ?>
          </span>
<?php	} else { ?>
          </div>
<?php	}
// }}}
	  } // endif(is_array(web_containers)
// }}}

	  // {{{ End of the span/div - host
	  if($_SESSION["opera"]) {
?>
        </span>
<?php } else { ?>
        </div>
<?php }

	  echo "\n";
// }}}
	} // end foreach($hosts)
  } // end if(is_array(hosts)
} // end if PQL_CONF_{CONTROL,WEBSRV,HOSTACL,SUDO}_USE

echo "\n";
require("./left-trailer.html");

pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
