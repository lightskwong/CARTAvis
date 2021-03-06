/**
 * Container for commands to hide specific images.
 */
/*global mImport */
/*******************************************************************************
 * @ignore( mImport)
 ******************************************************************************/

qx.Class.define("skel.Command.Data.CommandDataHide", {
    extend : skel.Command.CommandComposite,
    type : "singleton",

    /**
     * Constructor.
     */
    construct : function( ) {
        this.base( arguments, "Hide" );
        this.m_cmds = [];
        this.setEnabled( false );
        this.m_global = false;
        this.setToolTipText("Hide data...");
        this.setValue( this.m_cmds );
    },
    
    members : {
        /**
         * The commands to hide individual images have changed.
         */
        // Needed so that if data is added to an image that is already selected, i.e.,
        // enabled status has not changed, but data count has, the hide image commands
        // will be updated.
        datasChanged : function(){
            this._resetEnabled();
        },
        
        _resetEnabled : function( ){
            arguments.callee.base.apply( this, arguments );
            //Dynamically create hide image commands based on the active windows.
            this.m_cmds = [];
            var activeWins = skel.Command.Command.m_activeWins;
            if ( activeWins !== null && activeWins.length > 0 ){
                //Use the first one in the list that supports this cmd.
                var k = 0;
                var dataCmd = skel.Command.Data.CommandData.getInstance();
                for ( var i = 0; i < activeWins.length; i++ ){
                    if ( activeWins[i].isCmdSupported( dataCmd ) ){
                        var closes = activeWins[i].getDatas();
                        for ( var j = 0; j < closes.length; j++ ){
                            if ( closes[j].visible ){
                                this.m_cmds[k] = new skel.Command.Data.CommandDataHideImage( closes[j].name, closes[j].id);
                                k++;
                            }
                        }
                    }
                }
            }
            if ( this.m_cmds.length > 0 ){
                this.setEnabled( true );
            }
            else {
                this.setEnabled( false );
            }
            this.setValue( this.m_cmds );
            qx.event.message.Bus.dispatch(new qx.event.message.Message(
                    "commandsChanged", null));
        }
    }

});