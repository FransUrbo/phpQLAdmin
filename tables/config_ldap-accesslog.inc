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
                        <input type="text" name="start" size="14" value="<?php echo $_REQUEST["start"]?>">
                        <br>&nbsp;&nbsp;
                        <input type="text" name="end"   size="14" value="<?php echo $_REQUEST["end"]?>">
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
<?php for($i=$_REQUEST["start"]; $log[$i]; $i++) {
	$reqstart = pql_format_timestamp($log[$i]["reqstart"]);
	$reqstart = preg_replace('/ /', '&nbsp;', $reqstart);
	$reqstart = preg_replace('/\+.*/', '', $reqstart);

	$reqend   = pql_format_timestamp($log[$i]["reqend"]);
	$reqend   = preg_replace('/ /', '&nbsp;', $reqend);
	$reqend   = preg_replace('/\+.*/', '', $reqend);
?>

        <tr class="<?php echo pql_format_table()?>">
          <td align="right"><?php echo $i?></td>
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
       if(($_REQUEST["start"] > 0) && ($i >= ($_REQUEST["start"]*500))) {
	  break;
	}
      }
?>
      </th>
    </table>
