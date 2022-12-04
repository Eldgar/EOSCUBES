import { useStore } from "../hooks/useStore"
import * as React from 'react'
import { JsonRpc } from 'eosjs'
import { withUAL } from 'ual-reactjs-renderer/dist/components/provider/withUAL';
import { useState, useEffect } from 'react'



const Menu = (props) => {
	const [accountName, setAccountName] = useState(null)
    const [activeUser, setActiveUser] = useState(null)
    const [activeAuthenticator, setActiveAuthenticator] = useState(null)
	const [userBalance, setUserBalance] = useState(null)
    const [rpc, setRpc] = useState(new JsonRpc(`https://api-jungle.eosarabia.net:443`))
	const [saveWorld, resetWorld, setUal] = useStore((state) => [state.saveWorld, state.resetWorld, state.setUal])


	useEffect(() => { 
		const userId = async() => {
			 const accountName = await props.ual.activeUser.accountName
			 const activeAuthenticator = await props.ual.activeAuthenticator
			 const ual = await props.ual
			 const activeUser = await props.ual.activeUser
			  setAccountName(accountName)
			  setActiveUser(activeUser)
			  setActiveAuthenticator(activeAuthenticator)
			  setUal(ual);
			  
			  //console.log('test')
			  //console.log(accountName)
		  }
		  userId()
		const  fetchData = async() => {
			const response = await new JsonRpc(`https://api-jungle.eosarabia.net:443`).get_table_rows({
			json: true,
			code: 'eldgarcubes5',
			scope: 'eldgarcubes5',
			table: 'balances',
			limit: 10000,
			reverse: false,
			show_payer: false
		  })
		  const data = await response.rows
		  for (let i = 0; i < data.length; i++){
			if (data[i].user === parseInt(accountName)){
				setUserBalance(data[i].balance)
				return
			} 
		  }
		  //console.log("no user found")
		  
		}
		fetchData()
		  
	 },[props.ual])


	return (<div className="menu absolute">
		<button
			onClick={() => saveWorld()}
		>Save</button>
		<button
			onClick={() => resetWorld()}
		>Reset</button>
		{!accountName ? <div style={{ textAlign: 'center', padding: '5px' }}>
                <h2>Welcome!</h2> 
                <button onClick={props.ual.showModal}>Login</button>
            </div> : 
			<div style={{ textAlign: 'center', padding: '5px' }}>
            <h2>{accountName}</h2>
			{userBalance ? <h6>Balance: {userBalance}</h6> : <h6></h6> }
            <button onClick={props.ual.logout}>Logout</button>
        </div>}
	</div>)
}

export default withUAL(Menu)