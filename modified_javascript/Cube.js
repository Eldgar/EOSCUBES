import { useBox } from "@react-three/cannon"
import { useState } from "react"
import { useStore } from "../hooks/useStore"
import * as textures from "../images/textures"
import { RigidBody } from "@react-three/rapier"


export const Cube = ({ position, texture }) => {
	const [isHovered, setIsHovered] = useState(false)
	const [ref] = useBox(() => ({
		type: 'static',
		position,
		collisionFilterGroup: ((texture === 'saphire') ? 0x0004 : 0x0003),
		collisionFilterMask: ((texture === 'saphire') ? 0x0004 : 0x0003),
	}))
	const [addCube, removeCube] = useStore((state) => [state.addCube, state.removeCube])

	const activeTexture = textures[texture + 'Texture']



	return (
		<mesh
			onPointerMove={(e) => {
				e.stopPropagation()
				setIsHovered(true)
			}}
			onPointerOut={(e) => {
				e.stopPropagation()
				setIsHovered(false)
			}}
			onClick={(e) => {
				e.stopPropagation()
				const clickedFace = Math.floor(e.faceIndex / 2)
				const { x, y, z } = ref.current.position
				if (e.altKey) {
					removeCube(x, y, z)
					return
				}
				else if (clickedFace === 0) {
					addCube(x + 1, y, z)
					return
				}
				else if (clickedFace === 1) {
					addCube(x - 1, y, z)
					return
				}
				else if (clickedFace === 2) {
					addCube(x, y + 1, z)
					return
				}
				else if (clickedFace === 3) {
					addCube(x, y - 1, z)
					return
				}
				else if (clickedFace === 4) {
					addCube(x, y, z + 1)
					return
				}
				else if (clickedFace === 5) {
					addCube(x, y, z - 1)
					return
				}
			}}
			ref={ref}
		>
			<boxBufferGeometry attach="geometry" />
			<meshStandardMaterial
				color={isHovered ? 'grey' : 'white'}
				map={activeTexture}
				transparent={true}
				opacity={(texture === 'glass' || texture === 'saphire') ? 0.6 : 1}
				attach={"material"}
				/>
		</mesh>
	)
}