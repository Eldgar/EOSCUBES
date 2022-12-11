import { useEffect, useState } from 'react'
import { useStore } from "../hooks/useStore"
import { useKeyboard } from "../hooks/useKeyboard"
import { dirtImg, grassImg, glassImg, logImg, woodImg, saphireImg } from '../images/images'

const images = {
	dirt: dirtImg,
	grass: grassImg,
	glass: glassImg,
	wood: woodImg,
	log: logImg,
	saphire: saphireImg,
}

export const TextureSelector = () => {
	const [visible, setVisible] = useState(false)
	const [ activeUser, setActiveUser ] = useStore((state) => [state.activeUser, state.setActiveUser])
	const [activeTexture, setTexture, ual] = useStore((state) => [state.texture, state.setTexture, state.ual])
	const activeAccount = ual.activeUser
	const {
		dirt,
		grass,
		glass,
		wood,
		log,
		saphire,
	} = useKeyboard()
	if (true){
		useEffect(() => {
			const textures = {
				dirt,
				grass,
				glass,
				wood,
				log,
				saphire
			}
			const pressedTexture = Object.entries(textures).find(([k, v]) => v)
			if (pressedTexture) {
				setTexture(pressedTexture[0])
			}
		}, [setTexture, dirt, grass, glass, wood, log, saphire])
	} else {
		useEffect(() => {
			const textures = {
				dirt,
				grass,
				glass,
				wood,
				log,
			}
			const pressedTexture = Object.entries(textures).find(([k, v]) => v)
			if (pressedTexture) {
				setTexture(pressedTexture[0])
			}
		}, [setTexture, dirt, grass, glass, wood, log])
	
	}
	


	useEffect(() => {
		const visibilityTimeout = setTimeout(() => {
			setVisible(false)
		}, 2000)
		setVisible(true)
		return () => {
			clearTimeout(visibilityTimeout)
		}
	}, [activeTexture])

	return visible && (
		<div className='absolute centered texture-selector'>
			{Object.entries(images).map(([k, src]) => {
				return (<img
					key={k}
					src={src}
					alt={k}
					className={`${k === activeTexture ? 'active' : ''}`}
				/>)
			})}
		</div>
	)
}