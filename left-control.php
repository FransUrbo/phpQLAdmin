<?php
// navigation bar - controls information
// $Id: left-control.php,v 2.35.4.7 2006-11-17 13:53:25 turbo Exp $
//
// {{{ Setup session etc
require("./include/pql_session.inc");

require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/pql_control.inc");
require($_SESSION["path"]."/include/pql_control_plugins.inc");

require("./left-head.html");
// }}}

if((pql_get_define("PQL_CONF_CONTROL_USE") or
	pql_get_define("PQL_CONF_WEBSRV_USE") or
	pql_get_define("PQL_CONF_HOSTACL_USE") or
	pql_get_define("PQL_CONF_SUDO_USE")) and
   $_SESSION["ALLOW_CONTROL_CREATE"])
{
  // We're administrating QmailLDAP/Controls, Host ACLs, Sudoers or Webserver and
  // the user is allowed to administrate.
  $div_counter = 1;
  
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
  if($_SESSION["ADVANCED_MODE"]) {
	$host = split(';', $_SESSION["USER_HOST"]);
	
	// If it's an LDAP URI, replace "%2f" with "/" -> URLdecode
	$host[0] = urldecode($host[0]);
?>
    <!-- Add physical server link -->
    <div id="el<?=$div_counter?>Parent" class="parent">
      <a href="host_add.php">Add physical server/host</a>
    </div>

<?php
	$div_counter++;
  }
// }}}

  // {{{ Get all physical hosts
  $hosts = pql_get_dn($_pql->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
					  '(&(cn=*)(|(objectclass=ipHost)(objectclass=device)))',
					  'ONELEVEL');
  // }}}

  if(!is_array($hosts)) {
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
	// {{{ Add a 'Global' branch as first branch
	//     then add the rest of the hosts after that.
	$tmp[] = "Global";
	foreach($hosts as $host)
	  $tmp[] = $host;
	$hosts = $tmp;
// }}}

	foreach($hosts as $host_dn) {
	  // {{{ Get hostname for display
?>
          <!-- Physical server: <?=$host_dn?> -->
<?php
	  if($host_dn == 'Global')
		$host = 'Global';
	  else
		$host = pql_get_attribute($_pql->ldap_linkid, $host_dn, pql_get_define("PQL_ATTR_CN"));
// }}}

	  // {{{ Root of host tree
	  if($host_dn == 'Global') {
		pql_format_tree($host, "host_detail.php?host=".urlencode($host_dn)."&ref=left");
	  } else {
		$links = array();
		if(pql_get_define("PQL_CONF_CONTROL_USE")) {
		  $new = array($LANG->_('Add mail server') => "control_add_server.php?host=".urlencode($host_dn));
		  $links = $links + $new;
		}

		if(pql_get_define("PQL_CONF_WEBSRV_USE")) {
		  $new = array($LANG->_('Add web server')  => "websrv_add.php?view=websrv&host=".urlencode($host_dn));
		  $links = $links + $new;
		}

		pql_format_tree($host, "host_detail.php?host=".urlencode($host_dn)."&ref=left", $links, 0);
	  }
// }}}

	  // {{{ For each host - get QmailLDAP/Control hosts - if enabled
	  if(pql_get_define("PQL_CONF_CONTROL_USE")) {
		$qlcs = pql_get_dn($_pql->ldap_linkid, $host_dn, '(&(cn=*)(objectclass=qmailControl))', 'ONELEVEL');
		if(is_array($qlcs) or ($host_dn == 'Global')) {
		  // {{{ Got QLC Object(s)
		  if($host_dn == 'Global')
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
  <div id="el<?=$div_counter?>Child" class="child">
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
	  if(pql_get_define("PQL_CONF_WEBSRV_USE")) {
		$web_containers = pql_get_dn($_pql->ldap_linkid, $host_dn, '(&(cn=*)(objectclass=device))', 'ONELEVEL');
		if(is_array($web_containers) or ($host_dn == 'Global')) {
		  // {{{ Got Web container object(s)
		  if($host_dn == 'Global')
			$web_containers = array('Global'); // Fake this so that the foreach() won't complain.
		  
		  $links = array();
		  foreach($web_containers as $container_dn) {
			// {{{ Get container name for display
			if($host_dn == 'Global')
			  $container = 'Global';
			else
			  $container = pql_get_attribute($_pql->ldap_linkid, $container_dn, pql_get_define("PQL_ATTR_CN"));
// }}}

			// {{{ Extract the port number
			if(ereg(':', $container))
			  $port = ' - ' . $LANG->_('port') . ' ' . preg_replace('/.*:/', '', $container);
			elseif($host_dn == 'Global')
			  $port = ' - Global';
			else
			  $port = "";
			
			$container = pql_complete_constant($LANG->_('Webserver%port%'), array('port' => $port));
// }}}

			if($container_dn != 'Global') {
			  // {{{ Get Virtual host(s) for this web server container
			  $virt_hosts = pql_get_dn($_pql->ldap_linkid, $container_dn,
									   '(&(objectClass=ApacheVirtualHostObj)(objectClass=ApacheSectionObj))',
									   'ONELEVEL');
			  if(is_array($virt_hosts)) {
				// {{{ Go through the Virtual host(s)
				asort($virt_hosts);
				
				foreach($virt_hosts as $virt_dn) {
				  $virt = pql_get_attribute($_pql->ldap_linkid, $virt_dn, pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME"));
				  
				  // {{{ Setup the branch
				  $new = array($virt => "host_detail.php?host=".urlencode($host_dn)."&server=".urlencode($container_dn)."&virthost=".urlencode($virt)."&view=websrv&ref=left2");
				  $links = $links + $new;
// }}}
				} // end foreach virt_hosts
// }}}
			  } else {
				$new = array('no virtual hosts' => '');
				$links = $links + $new;
			  }
// }}}
			} else {
			  // {{{ Fake virtual host for the Global branch
			  $links = array('no virtual hosts' => '');
// }}}
			} // end if(container != Global)
			
			pql_format_tree($container, "host_detail.php?host=".urlencode($host_dn)."&server=".urlencode($container_dn)."&view=websrv&ref=left1", $links, 1);
		  } // end foreach($web_containers)
// }}}
		} else {
		  // {{{ No Web container object(s)
		  if($_SESSION["opera"]) {
?>

          <span id="el<?=$div_counter?>Spn" style="display:''">
<?php	  } else { ?>
          <div id="el<?=$div_counter?>Child" class="child">
<?php	  } ?>
            <nobr>&nbsp;&nbsp;&nbsp;
              <img src="images/navarrow.png" width="9" height="9" border="0">
              <font color="black" class="heada">no webserver defined</font>
            </nobr>
<?php	  if($_SESSION["opera"]) { ?>
          </span>
<?php	  } else { ?>
          </div>
<?php	  }
// }}}
		} // endif(is_array(web_containers)
	  }
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
