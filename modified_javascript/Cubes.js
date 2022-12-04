import { useStore } from '../hooks/useStore'
import { Cube } from './Cube'
import { JsonRpc } from 'eosjs'
import { useState, useEffect } from 'react';
import { withUAL } from 'ual-reactjs-renderer/dist/components/provider/withUAL';

export const Cubes = () => {
	const setLocalStorage = (key, value) => window.localStorage.setItem(key, JSON.stringify(value))
	const [table, setTable] = useState(null)
	useEffect(()=>{
		const  fetchData = async() => {
			const response = await new JsonRpc(`https://api-jungle.eosarabia.net:443`).get_table_rows({
			json: true,
			code: 'eldgarcubes5',
			scope: 'eldgarcubes5',
			table: 'cubes',
			limit: 10000,
			reverse: false,
			show_payer: false
		  })
		  const data = await response.rows
		  //console.log(data)
		  setTable(data)
		}
		fetchData()
		//const data = await JSON.stringify([...response.rows])
	},[])
	//console.log(table)
	//console.log(typeof(table))
	//console.log(state)
	const [cubes] = useStore((state) => [
		state.cubes
	])
	//console.log(cubes)
	//console.log(typeof(cubes))
	if (table){
		setLocalStorage('cubes', table)
		return cubes.map(({ key, pos, texture }) => {
			return (
				<Cube key={key} position={pos} texture={texture} /> 
			)
		})
	}
	
}


