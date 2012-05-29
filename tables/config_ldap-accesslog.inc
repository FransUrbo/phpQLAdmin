    <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post"
      <table cellspacing="0" cellpadding="3" border="0">
        <th colspan="3" align="left"><?php echo $LANG->_('LDAP Access Log')?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?php echo $LANG->_('Filter')?>:</td>
            <td>
              <table>
                <th>
                  <tr>
                    <td><?php echo $LANG->_('Type')?>&nbsp;</td>
                    <td>
                      &nbsp;<input type="checkbox" name="filter_type_bind"<?php if($_REQUEST["filter_type_bind"]) { echo " CHECKED"; } ?>><?php echo $LANG->_('Binds')?><br>
                      &nbsp;<input type="checkbox" name="filter_type_unbind"<?php if($_REQUEST["filter_type_unbind"]) { echo " CHECKED"; } ?>><?php echo $LANG->_('Unbinds')?><br>
                      &nbsp;<input type="checkbox" name="filter_type_search"<?php if($_REQUEST["filter_type_search"]) { echo " CHECKED"; } ?>><?php echo $LANG->_('Searches')?><br>
                      &nbsp;<input type="checkbox" name="filter_type_modify"<?php if($_REQUEST["filter_type_modify"]) { echo " CHECKED"; } ?>><?php echo $LANG->_('Modifications')?><br>
                      &nbsp;<input type="checkbox" name="filter_type_delete"<?php if($_REQUEST["filter_type_delete"]) { echo " CHECKED"; } ?>><?php echo $LANG->_('Deletions')?><br>
                    </td>
                  </tr>

                  <tr><td colspan="2"></td></tr>

                  <tr>
                    <td><?php echo $LANG->_('Session')?></td>
                    <td>#<input type="text" name="session" size="11" value="<?php echo $_REQUEST["session"]?>"></td>
                  </tr>

                  <tr><td colspan="2"></td></tr>

                  <tr>
                    <td><?php echo $LANG->_('LDAP Result value')?></td>
                    <td>&nbsp;&nbsp;
                      <select name="result_type">
                        <option value="is"<?php if($_REQUEST["result_type"] == 'is') { echo " SELECTED"; } ?>><?php echo $LANG->_('Is')?></option>
                        <option value="not"<?php if($_REQUEST["result_type"] == 'not') { echo " SELECTED"; } ?>><?php echo $LANG->_('Is Not')?></option>
                      </select>
                      <br>&nbsp;&nbsp;
                      <input type="text" name="result" size="11" value="<?php echo $_REQUEST["result"]?>">
                    </td>
                  </tr>

                  <tr><td colspan="2"></td></tr>

                  <tr>
                    <td><?php echo $LANG->_('Timeperiod')?></td>
		      <td>&nbsp;&nbsp;
                        <input type="text" name="start" size="14" value="<?php if(!@$_REQUEST["limit"]) { echo $_REQUEST["start"]; } ?>">
                        <br>&nbsp;&nbsp;
                        <input type="text" name="end"   size="14" value="<?php if(!@$_REQUEST["limit"]) { echo $_REQUEST["end"]; } ?>">
			<br>&nbsp;&nbsp;
                        <?php echo $LANG->_('Usage').": ".$LANG->_('YYYYMMDDHHMMSS')." (".$LANG->_('MM, DD, HH, MM and SS can be 0').")"; ?>
                    </td>
                  </tr>
                </th>
              </table>
            </td>

            <td>
              <input type="reset" name="action" value="&nbsp;<?php echo $LANG->_('Reset')?>&nbsp;">
              <br>
              <input type="submit" value="<?php echo $LANG->_('Search')?>">
            </td>
          </tr>
        </th>
      </table>

      <input type="hidden" name="filter" value="1">
      <input type="hidden" name="type"   value="<?php echo $_REQUEST["type"]?>">
    </form>

<?php
    if(count($log) > 2000) {
      if(!@$_REQUEST["cnt_start"]) {
	echo "<b>".$LANG->_('NOTE').": </b>".$LANG->_('Showing only the first 2000 records. Total:').count($log);
      } else {
	if(($_REQUEST["cnt_start"]+2000-1) > count($log))
	  $to = count($log);
	else
	  $to = $_REQUEST["cnt_start"]+2000-1;

	echo "<b>".$LANG->_('NOTE').": </b>".pql_complete_constant($LANG->_('Showing records between %from% and %to%. Total: %total%'),
								   array('from'  => $_REQUEST["cnt_start"],
									 'to'    => $to,
									 'total' => count($log)));
      }
    }
?>
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">
        <tr class="title">
          <td><!-- counter --></td>
          <td><?php echo $LANG->_('Start')?></td>
          <td><?php echo $LANG->_('End')?></td>
          <td><?php echo $LANG->_('Session')?></td>
          <td><?php echo $LANG->_('Type')?></td>
          <td><?php echo $LANG->_('Result')?></td>
          <td><?php echo $LANG->_('Who')?>/<?php echo $LANG->_('What')?></td>
        </tr>
<?php
    // What records to output (how many)
    if(!@$_REQUEST["filter"] && @$_REQUEST["start"]) {
      $start = $_REQUEST["start"];
      if(@$_REQUEST["end"])
	$end = $_REQUEST["end"];
      else
	$end = 2000;
    } elseif(@$_REQUEST["cnt_start"]) {
      $start = $_REQUEST["cnt_start"];
      $end   = $start + 2000;
    } else {
      $start = 0;
      $end   = 2000;
    }

    for($i=$start; $log[$i] && ($i < $end); $i++) {
	$reqstart = pql_format_timestamp($log[$i]["reqstart"]);
	$reqstart = preg_replace('/ /', '&nbsp;', $reqstart);
	$reqstart = preg_replace('/\+.*/', '', $reqstart);

	$reqend   = pql_format_timestamp($log[$i]["reqend"]);
	$reqend   = preg_replace('/ /', '&nbsp;', $reqend);
	$reqend   = preg_replace('/\+.*/', '', $reqend);
?>

        <tr class="<?php echo pql_format_table()?>">
          <td align="right"><!-- counter = <?php echo $i?> --></td>
          <td><?php echo $reqstart?>&nbsp;&nbsp;</td>
          <td><?php echo $reqend?>&nbsp;&nbsp;</td>
          <td><?php echo $log[$i]["reqsession"]?></td>
          <td><?php echo $log[$i]["reqtype"]?></td>
          <td align="center"><?php echo $log[$i]["reqresult"]?></td>
          <td>
<?php	if($log[$i]["reqauthzid"]) { ?>
<?php	  if($log[$i]["reqdn"]) { ?>AuthzID:<?php } ?>
            <?php echo $log[$i]["reqauthzid"]?>
<?php	  if($log[$i]["reqdn"]) { ?><br><?php } ?>
<?php	} 

	if($log[$i]["reqtype"] == 'search') {
?>
            Scope: '<?php echo $log[$i]["reqscope"]?>', Filter: '<?php echo $log[$i]["reqfilter"]?>'
<?php	  if($log[$i]["reqdn"]) { ?><br><?php } ?>
<?php	} ?>
            <?php echo $log[$i]["reqdn"]?>
          </td>
        </tr>
<?php
	if(!@$_REQUEST["limit"] &&
	   ($_REQUEST["start"] > 0) &&
	   ($i >= ($_REQUEST["start"]*500))) {
	  // No limit set,
	  // ... Start is > 0
	  // ... check $i against max start
	  echo "!_REQUEST[limit] && (_REQUEST[start] > 0) && (i >= _REQUEST[start]*500)<br>";
	  break;
	}
      }
?>
      </th>
    </table>

<?php if((count($log) > 2000) && ($i < count($log))) { ?>
    <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
      <input type="hidden" name="type"        value="<?php echo $_REQUEST["type"]?>">
<?php if(@$_REQUEST["filter"]) { ?>
      <input type="hidden" name="result_type" value="<?php echo $_REQUEST["result_type"]?>">
      <input type="hidden" name="result"      value="<?php echo $_REQUEST["result"]?>">
      <input type="hidden" name="start"       value="<?php echo $_REQUEST["start"]?>">
      <input type="hidden" name="end"         value="<?php echo $_REQUEST["end"]?>">
      <input type="hidden" name="filter"      value="<?php echo $_REQUEST["filter"]?>">
<?php } ?>
      <input type="hidden" name="cnt_start"   value="<?php echo $start+2000?>">
      <input type="submit" value="<?php echo $LANG->_('Next 2000 entries')?>">
    </form>
<?php } ?>
